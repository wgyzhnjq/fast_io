#pragma once
#include"ryu/ryu.h"
//#include"grisu_exact/grisu_exact.h"
//#include"grisu_exact/grisu_exact_api.h"

namespace fast_io
{

template<buffer_input_stream input,std::floating_point T>
inline constexpr bool scan_define(input& in,T &t)
{
	if(!skip_space(in))
		return false;
	auto igen{igenerator(in)};
	t=static_cast<std::remove_cvref_t<T>>(details::ryu::input_floating<u8'.',double>(begin(igen),end(igen)));
	return true;
}

template<char32_t dec,buffer_input_stream input,std::floating_point T>
inline constexpr bool scan_define(input& in,manip::decimal_point<T&,dec> t)
{
	if(!skip_space(in))
		return false;
	auto igen{igenerator(in)};
	t.value=static_cast<std::remove_cvref_t<T>>(details::ryu::input_floating<dec,double>(begin(igen),end(igen)));
	return true;
}

/*
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

		auto start(oreserve(out,reserved_size));
		if constexpr(std::is_pointer_v<decltype(start)>)
		{
			if(start)
			{
				orelease(out,details::ryu::output_fixed<precision,true,uppercase_e>(start,static_cast<double>(a.reference)));
				return;
			}
		}
		else
		{
			orelease(out,details::ryu::output_fixed<precision,true,uppercase_e>(start,static_cast<double>(a.reference)));
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

template<std::floating_point T>
inline constexpr std::size_t print_reserve_size(print_reserve_type_t<manip::fixed_shortest<T>>)
{
	return 325;
}
*/
/*
template<std::random_access_iterator raiter,std::floating_point T,typename P>
inline raiter print_reserve_define(print_reserve_type_t<manip::fixed_shortest<T>>,raiter start,P a)
{
	return details::ryu::output_shortest<false,1>(start,static_cast<double>(a.reference));
}

template<bool uppercase_e,std::floating_point T>
inline constexpr std::size_t print_reserve_size(print_reserve_type_t<manip::scientific_shortest<uppercase_e,T>>)
{
	return 30;
}

template<std::random_access_iterator raiter,bool uppercase_e,std::floating_point T,typename P>
inline raiter print_reserve_define(print_reserve_type_t<manip::scientific_shortest<uppercase_e,T>>,raiter start,P a)
{
	return details::ryu::output_shortest<uppercase_e,2>(start,static_cast<double>(a.reference));
}

template<bool uppercase_e,std::floating_point T>
inline constexpr std::size_t print_reserve_size(print_reserve_type_t<manip::general_shortest<uppercase_e,T>>)
{
	return 30;
}

template<std::random_access_iterator raiter,bool uppercase_e,std::floating_point T,typename P>
inline raiter print_reserve_define(print_reserve_type_t<manip::general_shortest<uppercase_e,T>>,raiter start,P a)
{
	return details::ryu::output_shortest<uppercase_e>(start,static_cast<double>(a.reference));
}

template<std::floating_point T>
inline constexpr std::size_t print_reserve_size(print_reserve_type_t<manip::int_hint<T>>)
{
	return 30;
}

template<std::random_access_iterator raiter,std::floating_point T,typename P>
inline raiter print_reserve_define(print_reserve_type_t<manip::int_hint<T>>,raiter start,P a)
{
	return details::ryu::output_shortest<false,0,true>(start,static_cast<double>(a.reference));
}
*/

template<manip::floating_formats fm,bool uppercase,std::floating_point T,char32_t dec>
inline constexpr std::size_t print_reserve_size
	(print_reserve_type_t<manip::decimal_point<manip::floating_manip_precision<fm,uppercase,T const>,dec>>)
{
	if constexpr(fm==manip::floating_formats::general||fm==manip::floating_formats::fixed)
		return 2048;
	else if constexpr(fm==manip::floating_formats::scientific)
		return 1024;
}

template<std::random_access_iterator raiter,manip::floating_formats fm,std::floating_point T,bool uppercase,char32_t dec,typename U>
requires (dec<std::numeric_limits<std::iter_value_t<raiter>>::max())
inline raiter print_reserve_define(print_reserve_type_t<manip::decimal_point<manip::floating_manip_precision<fm,uppercase,T const>,dec>>,raiter start,U a)
{
	if constexpr(fm==manip::floating_formats::fixed)
	{
		if(1024<a.value.precision)
			throw std::runtime_error("precision too large");
		return details::ryu::output_fixed<dec,false,uppercase>(start,a.value.reference,a.value.precision);
	}
	else if constexpr(fm==manip::floating_formats::scientific)
	{
		if(512<a.value.precision)
			throw std::runtime_error("precision too large");
		return details::ryu::output_fixed<dec,true,uppercase>(start,a.value.reference,a.value.precision);
	}
	else if constexpr(fm==manip::floating_formats::general)
	{
		if(1024<a.value.precision)
			throw std::runtime_error("precision too large");
		auto fixed_iter{details::ryu::output_fixed<dec,false,uppercase>(start,a.value.reference,a.value.precision)};
		std::array<std::iter_value_t<raiter>,512> scientific;
		auto scientific_it{details::ryu::output_fixed<dec,true,uppercase>(scientific.data(),a.value.reference,a.value.precision)};
		if(scientific_it-scientific.data()<fixed_iter-start)[[unlikely]]
		{
			details::my_copy(scientific.data(),scientific_it,start);
			return start+(scientific_it-scientific.data());
		}
		return fixed_iter;
	}
}

template<std::floating_point T>
inline constexpr std::size_t print_reserve_size(print_reserve_type_t<T>)
{
	return 30;
}

template<std::random_access_iterator raiter,std::floating_point T>
inline raiter print_reserve_define(print_reserve_type_t<T>,raiter start,T a)
{
	return details::ryu::output_shortest<u8'.',false,0,true>(start,static_cast<double>(a));
}

template<manip::floating_formats fm,bool uppercase,std::floating_point T,char32_t dec>
inline constexpr std::size_t print_reserve_size
	(print_reserve_type_t<manip::decimal_point<manip::floating_manip<fm,uppercase,T const>,dec>>)
{
	if constexpr(fm==manip::floating_formats::general||fm==manip::floating_formats::scientific)
		return 30;
	else if constexpr(fm==manip::floating_formats::fixed)
		return 325;
}

template<std::random_access_iterator raiter,manip::floating_formats fm,std::floating_point T,bool uppercase,char32_t dec,typename U>
requires (dec<std::numeric_limits<std::iter_value_t<raiter>>::max())
inline raiter print_reserve_define(print_reserve_type_t<manip::decimal_point<manip::floating_manip<fm,uppercase,T const>,dec>>,raiter start,U a)
{
	if constexpr(fm==manip::floating_formats::general)
		return details::ryu::output_shortest<dec,uppercase,0,true>(start,static_cast<double>(a.value.reference));
	else if constexpr(fm==manip::floating_formats::fixed)
		return details::ryu::output_shortest<dec,false,1,true>(start,static_cast<double>(a.value.reference));
	else if constexpr(fm==manip::floating_formats::scientific)
		return details::ryu::output_shortest<dec,uppercase,2,true>(start,static_cast<double>(a.value.reference));
}

template<std::floating_point T,char32_t dec>
inline constexpr std::size_t print_reserve_size
	(print_reserve_type_t<manip::decimal_point<T&,dec>>)
{
	return 30;
}

template<std::random_access_iterator raiter,std::floating_point T,char32_t dec,typename U>
requires (dec<std::numeric_limits<std::iter_value_t<raiter>>::max())
inline raiter print_reserve_define(print_reserve_type_t<manip::decimal_point<T&,dec>>,raiter start,U a)
{
	return details::ryu::output_shortest<dec,false,0,true>(start,static_cast<double>(a.value));
}

}
