#pragma once

namespace fast_io
{
namespace details
{
template<output_stream output,input_stream input>
inline constexpr std::size_t bufferred_transmit_impl(output& outp,input& inp)
{
	std::size_t transmitted_bytes{};
	if constexpr(buffer_input_stream<input>)
	{
		do
		{
			auto b(begin(inp));
			auto e(end(inp));
			std::size_t transmitted_this_round(static_cast<std::size_t>(e-b));
			write(outp,b,e);
			transmitted_bytes+=transmitted_this_round;
		}
		while(iflush(inp));
		return transmitted_bytes;
	}
	else
	{
		for(std::array<std::byte,65536> array;;)
		{
			auto p(read(inp,array.data(),array.data()+array.size()));
			std::size_t transmitted_this_round(p-array.data());
			transmitted_bytes+=transmitted_this_round;
			write(outp,array.data(),p);
			if(!transmitted_this_round)
				return transmitted_bytes;
		}
	}
}

template<output_stream output,input_stream input>
inline constexpr std::size_t bufferred_transmit_impl(output& outp,input& inp,std::size_t bytes)
{
	std::size_t transmitted_bytes{};
	if constexpr(buffer_input_stream<input>)
	{
		do
		{
			auto b(begin(inp));
			auto e(end(inp));
			if(b!=e)[[likely]]
			{
				std::size_t transmitted_this_round((e-b)*sizeof(*b));
				if(bytes<=transmitted_this_round)
				{
					write(outp,b,bytes);
					return transmitted_bytes+transmitted_this_round;
				}
				else
				{
					write(outp,b,e);
					transmitted_bytes+=transmitted_this_round;
					bytes-=transmitted_this_round;
				}
			}
		}
		while(iflush(inp));
		return transmitted_bytes;
	}
	else
	{
		for(std::array<std::byte,65536> array;bytes;)
		{
			std::size_t b(array.size());
			if(bytes<b)
				b=bytes;
			auto p(read(inp,array.data(),array.data()+b));
			std::size_t read_bytes(p-array.data());
			write(outp,array.data(),p);
			transmitted_bytes+=read_bytes;
			if(read_bytes!=b)
				return transmitted_bytes;
			bytes-=read_bytes;
		}
		return transmitted_bytes;
	}
}
#ifdef __linux__
template<output_stream output,input_stream input>
inline constexpr std::size_t zero_copy_transmit_impl(output& outp,input& inp)
{
	auto ret(zero_copy_transmit<true>(outp,inp));
	if(ret.second)
		return ret.first+bufferred_transmit_impl(outp,inp);
	return ret.first;
}

template<output_stream output,input_stream input>
inline constexpr std::size_t zero_copy_transmit_impl(output& outp,input& inp,std::size_t sz)
{
	auto ret(zero_copy_transmit<true>(outp,inp,sz)); 
	if(ret.second)
		return ret.first+bufferred_transmit_impl(outp,inp,sz-ret.first);
	return ret.first;
}
#endif

template<output_stream output,input_stream input,typename... Args>
inline constexpr auto transmit_impl(output& outp,input& inp,Args&& ...args)
{
	if constexpr(mutex_input_stream<input>)
	{
		typename input::lock_guard_type lg{mutex(inp)};
		decltype(auto) uh{unlocked_handle(inp)};
		return transmit_impl(outp,uh,std::forward<Args>(args)...);
	}
	else
	{
#ifdef __cpp_lib_is_constant_evaluated
		if (std::is_constant_evaluated())
			return bufferred_transmit_impl(outp,inp,std::forward<Args>(args)...);
		else
		{
#endif
		if constexpr(zero_copy_output_stream<output>&&zero_copy_input_stream<input>)
		{
			if constexpr(buffer_input_stream<input>)
			{
				write(outp,begin(inp),end(inp));
				iclear(inp);
			}
			if constexpr(buffer_output_stream<output>)
				flush(outp);
#ifdef __linux__
			return zero_copy_transmit_impl(outp,inp,std::forward<Args>(args)...);
#else
			return zero_copy_transmit(outp,inp,std::forward<Args>(args)...);
#endif
		}
#ifdef __cpp_lib_is_constant_evaluated
		else
			return bufferred_transmit_impl(outp,inp,std::forward<Args>(args)...);
		}
#endif
	}
}
}

template<output_stream output,input_stream input,std::integral sz_type>
inline constexpr void print_define(output& outp,manip::transmission<input,sz_type> ref)
{
	ref.transmitted=static_cast<sz_type>(details::transmit_impl(outp,ref.reference));
}

template<output_stream output,input_stream input,std::integral sz_type>
inline constexpr void print_define(output& outp,manip::transmission_with_size<input,sz_type> ref)
{
	ref.transmitted=static_cast<sz_type>(details::transmit_impl(outp,ref.reference,ref.bytes));
}

template<output_stream output,input_stream input,std::integral sz_type>
inline constexpr sz_type transmit(output& outp,input& in,sz_type s)
{
	sz_type transmitted{};
	print(outp,manip::transmission_with_size<input,sz_type>{transmitted,in,s});
	return transmitted;
}

template<output_stream output,input_stream input>
inline constexpr std::size_t transmit(output& outp,input& in)
{
	std::size_t transmitted{};
	print(outp,manip::transmission<input,std::size_t>(transmitted,in));
	return transmitted;
}

}
