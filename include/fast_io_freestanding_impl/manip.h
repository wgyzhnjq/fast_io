#pragma once

namespace fast_io
{

namespace details
{

template<character_output_stream output,typename T,std::unsigned_integral W,std::integral U>
inline constexpr void print_width(output& out,T const& reference,W width,U ch)
{
	basic_ostring<std::basic_string<typename output::char_type>> bas;
	print(bas,reference);
	std::size_t const size(bas.str().size());
	if(size<width)
		fill_nc(out,width-size,ch);
	print(out,bas.str());
}

}

template<character_output_stream output,typename T,std::integral U>
inline constexpr void print_define(output& out,manip::setw_fill_t<T,U> a)
{
	return details::print_width(out,a.reference,a.width,a.ch);
}

template<character_output_stream output,typename T>
inline constexpr void print_define(output& out,manip::setw_t<T> a)
{
	return details::print_width(out,a.reference,a.width,' ');
}


template<character_output_stream output,typename T,std::integral U>
inline constexpr void print_define(output& out,manip::setz_t<T> a)
{
	return details::print_width(out,a.reference,a.width,'0');
}

}