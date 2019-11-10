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

template<character_input_stream input>
inline void scan_define(input& in,std::basic_string<typename input::char_type> &str)
{
	str.clear();
	str.push_back(eat_space_get(in));
	for(decltype(try_get(in)) ch;!(ch=try_get(in)).second&&!details::isspace(ch.first);str.push_back(ch.first));
}

template<character_input_stream input>
inline void getline(input& in,std::basic_string<typename input::char_type> &str)
{
	str.clear();
	for(decltype(try_get(in)) ch;!(ch=try_get(in)).second&&ch.first!='\n';str.push_back(ch.first));
}

template<character_input_stream input>
inline void getwhole(input& in,std::basic_string<typename input::char_type> &str)
{
	str.clear();
	for(decltype(try_get(in)) ch;!(ch=try_get(in)).second;str.push_back(ch.first));
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