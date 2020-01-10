#pragma once

namespace fast_io
{

template<output_stream output,typename func>
class otransform
{
public:
	using native_handle_type = output; 
	using transform_function_type = func;
	using char_type = typename func::char_type;
	native_handle_type handle;
	transform_function_type transform_function;
	std::size_t position{};
	alignas(4096*sizeof(char_type)) std::array<char_type,4096> buffer;
	auto& native_handle()
	{
		return handle;
	}
private:
	void close()
	{
		try
		{
			if(position!=buffer.size())
				transform_function(handle,buffer.data(),buffer.data()+buffer.size());
		}
		catch(...){}
	}
public:
	otransform(otransform const&) = default;
	otransform& operator=(otransform const&) = default;
	otransform(otransform&& other) noexcept:handle(std::move(other.handle)),
			transform_function(std::move(other.transform_function)),
			position(std::move(other.position)),
			buffer(std::move(other.buffer))
	{
		other.position=buffer.size();
	}
	otransform& operator=(otransform&& other) noexcept
	{
		if(std::addressof(other)!=this)
		{
			close();
			handle=std::move(other.handle);
			transform_function=std::move(other.transform_function);
			position=std::move(other.position);
			buffer=std::move(other.buffer);
			other.position=buffer.size();
		}
		return *this;
	}
	~otransform()
	{
		close();
	}
};
namespace details
{

template<typename T,std::contiguous_iterator Iter>
inline constexpr void otransform_write(T& ob,Iter cbegin,Iter cend)
{
	std::size_t const diff(cend-cbegin);
	if(ob.buffer.size()<=ob.position+diff)[[unlikely]]
	{
		cbegin=std::copy_n(cbegin,ob.size()-ob.position,ob.buffer.data()+ob.position);
		for(ob.transform_function(ob.handle,ob.buffer.data(),ob.buffer.data()+ob.buffer.size());
			std::copy(cbegin,cend,ob.buffer.data(),ob.buffer.size())==ob.buffer.data()+ob.buffer.size();
			ob.transform_function(ob.handle,ob.buffer.data(),ob.buffer.data()+ob.buffer.size()));
	}
	std::copy_n(cbegin,diff,ob.buffer.data()+ob.position);
	ob.position+=diff;
}

}

template<output_stream Ohandler,typename func,std::contiguous_iterator Iter>
inline constexpr void write(otransform<Ohandler,func>& ob,Iter cbegini,Iter cendi)
{
	using char_type = typename otransform<Ohandler,func>::char_type;
	if constexpr(std::same_as<char_type,typename std::iterator_traits<Iter>::value_type>)
		details::otransform_write(ob,std::to_address(cbegini),std::to_address(cendi));
	else
		details::otransform_write(ob,reinterpret_cast<std::byte const*>(std::to_address(cbegini)),
					reinterpret_cast<std::byte const*>(std::to_address(cendi)));
}

template<output_stream Ohandler,typename func>
inline constexpr void flush(otransform<Ohandler,func>& ob)
{
	ob.transform_function(ob.handle,ob.buffer.data(),ob.buffer.data()+ob.position);
}

template<buffer_input_stream Ohandler,typename func>
inline constexpr decltype(auto) iflush(otransform<Ohandler,func>& out)
{
	return iflush(*out);
}

template<buffer_input_stream Ohandler,typename func>
inline constexpr decltype(auto) begin(otransform<Ohandler,func>& out)
{
	return begin(out.native_handle());
}

template<buffer_input_stream Ohandler,typename func>
inline constexpr decltype(auto) end(otransform<Ohandler,func>& out)
{
	return end(out.native_handle());
}

template<buffer_input_stream Ohandler,typename func>
inline constexpr otransform<Ohandler,func>& operator++(otransform<Ohandler,func>& out)
{
	operator++(out.native_handle());
	return out;
}

template<buffer_input_stream Ohandler,typename func,std::integral I>
inline constexpr otransform<Ohandler,func>& operator+=(otransform<Ohandler,func>& out,I i)
{
	operator+=(out.native_handle(),i);
	return out;
}

template<output_stream Ohandler,typename func,std::integral I>
[[nodiscard]] inline constexpr auto oreserve(otransform<Ohandler,func>& ob,I size) -> decltype(ob.buffer.data())
{
	if(ob.buffer.size()<=ob.position+size)
		return nullptr;
	return ob.position+=size;
}

template<output_stream Ohandler,typename func,std::integral I>
inline constexpr void orelease(otransform<Ohandler,func>& ob,I size)
{
	ob.position-=size;
}

template<output_stream Ohandler,typename func>
inline constexpr void put(otransform<Ohandler,func>& ob,typename otransform<Ohandler,func>::char_type ch)
{
	if(ob.position==ob.buffer.size())[[unlikely]]		//buffer full
	{
		ob.transform_function(ob.handle,ob.buffer.data(),ob.buffer.data()+ob.position);
		ob.position=1;
		ob.buffer.front()=ch;
		return;//no flow dependency any more
	}
	*ob.position=ch;
	++ob.position;
}

}