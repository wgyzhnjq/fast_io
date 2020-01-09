#pragma once
#include"ryu/ryu.h"

namespace fast_io
{

template<output_stream output,std::size_t precision,std::floating_point T>
inline void print_define(output& out,manip::fixed<precision,T const> a)
{

	std::size_t constexpr reserved_size(precision+325);
	if constexpr(buffer_output_stream<output>)
	{

		auto reserved(oreserve(out,reserved_size));
		if constexpr(std::is_pointer_v<decltype(reserved)>)
		{
			if(reserved)
			{
				auto start(reserved-reserved_size);
				orelease(out,reserved-details::ryu::output_fixed<precision>(start,static_cast<double>(a.reference)));
				return;
			}
		}
		else
		{
			auto start(reserved-reserved_size);
			orelease(out,reserved-details::ryu::output_fixed<precision>(start,static_cast<double>(a.reference)));
			return;
		}
	}
	if constexpr (precision<325)
	{
		std::array<typename output::char_type,reserved_size> array;
		write(out,array.data(),details::ryu::output_fixed<precision>(array.data(),static_cast<double>(a.reference)));
	}
	else
	{
		std::basic_string<typename output::char_type> str(reserved_size);
		write(out,str.data(),details::ryu::output_fixed<precision>(str.data(),static_cast<double>(a.reference)));
	}
}

template<output_stream output,std::size_t precision,bool uppercase_e,std::floating_point T>
inline void print_define(output& out,manip::scientific<precision,uppercase_e,T const> a)
{

	std::size_t constexpr reserved_size(precision+10);
	if constexpr(buffer_output_stream<output>)
	{

		auto reserved(oreserve(out,reserved_size));
		if constexpr(std::is_pointer_v<decltype(reserved)>)
		{
			if(reserved)
			{
				auto start(reserved-reserved_size);
				orelease(out,reserved-details::ryu::output_fixed<precision,true,uppercase_e>(start,static_cast<double>(a.reference)));
				return;
			}
		}
		else
		{
			auto start(reserved-reserved_size);
			orelease(out,reserved-details::ryu::output_fixed<precision,true,uppercase_e>(static_cast<double>(a.reference)));
			return;
		}
	}
	if constexpr (precision<325)
	{
		std::array<typename output::char_type,reserved_size> array;
		write(out,array.data(),details::ryu::output_fixed<precision,true,uppercase_e>(array.data(),static_cast<double>(a.reference)));
	}
	else
	{
		std::basic_string<typename output::char_type> str(reserved_size);
		write(out,str.data(),details::ryu::output_fixed<precision,true,uppercase_e>(str.data(),static_cast<double>(a.reference)));
	}
}

template<output_stream output,std::floating_point T>
inline void print_define(output& out,manip::fixed_shortest<T const> a)
{

	std::size_t constexpr reserved_size(325);
	if constexpr(buffer_output_stream<output>)
	{

		auto reserved(oreserve(out,reserved_size));
		if constexpr(std::is_pointer_v<decltype(reserved)>)
		{
			if(reserved)
			{
				auto start(reserved-reserved_size);
				orelease(out,reserved-details::ryu::output_shortest<false,1>(start,static_cast<double>(a.reference)));
				return;
			}
		}
		else
		{
			auto start(reserved-reserved_size);
			orelease(out,reserved-details::ryu::output_shortest<false,1>(start,static_cast<double>(a.reference)));
			return;
		}
	}
	std::array<typename output::char_type,reserved_size> array;
	write(out,array.data(),details::ryu::output_shortest<false,1>(array.data(),static_cast<double>(a.reference)));
}

template<output_stream output,bool uppercase_e,std::floating_point T>
inline void print_define(output& out,manip::scientific_shortest<uppercase_e,T const> a)
{

	std::size_t constexpr reserved_size(30);
	if constexpr(buffer_output_stream<output>)
	{

		auto reserved(oreserve(out,reserved_size));
		if constexpr(std::is_pointer_v<decltype(reserved)>)
		{
			if(reserved)
			{
				auto start(reserved-reserved_size);
				orelease(out,reserved-details::ryu::output_shortest<uppercase_e,2>(start,static_cast<double>(a.reference)));
				return;
			}
		}
		else
		{
			auto start(reserved-reserved_size);
			orelease(out,reserved-details::ryu::output_shortest<uppercase_e,2>(start,static_cast<double>(a.reference)));
			return;
		}
	}
	std::array<typename output::char_type,reserved_size> array;
	write(out,array.data(),details::ryu::output_shortest<uppercase_e,2>(array.data(),static_cast<double>(a.reference)));
}

template<output_stream output,bool uppercase_e,std::floating_point T>
inline void print_define(output& out,manip::shortest_shortest<uppercase_e,T const> a)
{

	std::size_t constexpr reserved_size(30);
	if constexpr(buffer_output_stream<output>)
	{

		auto reserved(oreserve(out,reserved_size));
		if constexpr(std::is_pointer_v<decltype(reserved)>)
		{
			if(reserved)
			{
				auto start(reserved-reserved_size);
				orelease(out,reserved-details::ryu::output_shortest<uppercase_e>(start,static_cast<double>(a.reference)));
				return;
			}
		}
		else
		{
			auto start(reserved-reserved_size);
			orelease(out,reserved-details::ryu::output_shortest<uppercase_e>(start,static_cast<double>(a.reference)));
			return;
		}
	}
	std::array<typename output::char_type,reserved_size> array;
	write(out,array.data(),details::ryu::output_shortest<uppercase_e>(array.data(),static_cast<double>(a.reference)));
}

template<output_stream output,std::floating_point T>
inline void print_define(output& out,T a)
{
	std::size_t constexpr reserved_size(30);
	if constexpr(buffer_output_stream<output>)
	{

		auto reserved(oreserve(out,reserved_size));
		if constexpr(std::is_pointer_v<decltype(reserved)>)
		{
			if(reserved)
			{
				auto start(reserved-reserved_size);
				orelease(out,reserved-details::ryu::output_shortest<false>(start,static_cast<double>(a)));
				return;
			}
		}
		else
		{
			auto start(reserved-reserved_size);
			orelease(out,reserved-details::ryu::output_shortest<false>(start,static_cast<double>(a)));
			return;
		}
	}
	std::array<typename output::char_type,reserved_size> array;
	write(out,array.data(),details::ryu::output_shortest<false>(array.data(),static_cast<double>(a)));
}

template<buffer_input_stream input,std::floating_point T>
inline constexpr bool scan_define(input& in,T &t)
{
/*	auto sp(iview(in));
	auto i(sp.data()),e(sp.data()+sp.size());
	bool minus{};
	for(;;)
	{
		for(;i!=e;++i)
			if(*i==u8'-'&&*i==u8'.'&&static_cast<char8_t>(*i-u8'0')<static_cast<char8_t>(0xA))
				break;
		if(i!=e)
		{
			if(*i==u8'-')
			{
				if(++i==e)
				{
					if(!iflush(in))
						return false;
					sp=iview(in);
					i=sp.data();
					e=sp.data()+sp.size();
				}
				if(*i!=u8'.'&&static_cast<char8_t>(0x9)<static_cast<char8_t>(*i-u8'0'))
					continue;
				minus=true;
			}
			else if(*i==u8'.')
			{
				if(++i==e)
				{
					if(!iflush(in))
						return false;
					sp=iview(in);
					i=sp.data();
					e=sp.data()+sp.size();
				}
				if(static_cast<char8_t>(0x9)<static_cast<char8_t>(*i-u8'0'))
					continue;
			}
			break;
		}
		if(!iflush(in))
			return false;
		sp=iview(in);
		i=sp.data();
		e=sp.data()+sp.size();
	}
	
	if(i+1==e)
	{
		return false;
	}*/
	if(!skip_space(in))
		return false;
	t=static_cast<std::remove_cvref_t<T>>(details::ryu::input_floating<double>(in));
	return true;
//	t=static_cast<std::remove_cvref_t<T>>(details::ryu::input_floating<double>(in));
}

}
