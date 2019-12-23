#pragma once

namespace fast_io
{
/*We have no choice but to assume it is ansi based. Even On EBCDIC systems
since we cannot prevent people link from other libraries using ANSI that throw exception*/

template<character_output_stream output>
requires (std::same_as<typename output::char_type,char8_t>||std::same_as<typename output::char_type,char>)
inline void print_define(output& out,std::exception const &e)
{
	print(out,from_utf8_unchecked(e.what()));
}

template<character_output_stream output>
requires (std::same_as<typename output::char_type,char8_t>||std::same_as<typename output::char_type,char>)
inline void print_define(output& out,std::system_error const &e)
{
	auto const& code(e.code());
	if constexpr(std::same_as<typename output::char_type,char8_t>)
	{
		print(out,u8"std::system_error, value:",code.value(),u8"\tmessage:",from_utf8_unchecked(code.message()),u8"\twhat:",from_utf8_unchecked(e.what()));
	}
	else if constexpr(std::same_as<typename output::char_type,char>)
		print(out,"std::system_error, value:",code.value(),"\tmessage:",code.message(),"\twhat:",e.what());
}

}