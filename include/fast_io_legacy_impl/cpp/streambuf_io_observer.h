#pragma once

#include<streambuf>
#include<fstream>
#include<sstream>

namespace fast_io
{

template<typename T>
requires std::derived_from<T,std::basic_streambuf<typename T::char_type,typename T::traits_type>>
class basic_general_streambuf_io_observer
{
public:
	using streambuf_type = T;
	using char_type = typename streambuf_type::char_type;
	using traits_type = typename streambuf_type::traits_type;
	using native_handle_type = streambuf_type*;
	native_handle_type rdb{};
	inline constexpr native_handle_type& native_handle() const noexcept
	{
		return rdb;
	}
	inline constexpr native_handle_type& native_handle() noexcept
	{
		return rdb;
	}
};

template<typename T,std::contiguous_iterator Iter>
inline Iter read(basic_general_streambuf_io_observer<T>& t,Iter begin,Iter end)
{
	using char_type = typename T::char_type;
	return begin+(t.rdb->sgetn(static_cast<char_type*>(static_cast<void*>(std::to_address(begin))),(end-begin)*sizeof(*begin)/sizeof(char_type))*sizeof(char_type)/sizeof(*begin));
}

template<typename T,std::contiguous_iterator Iter>
inline Iter write(basic_general_streambuf_io_observer<T> t,Iter begin,Iter end)
{
	using char_type = typename T::char_type;
/*
	if(!t.rdb->sputn(static_cast<char_type const*>(static_cast<void const*>(std::to_address(begin))),(end-begin)*sizeof(*begin)/sizeof(char_type)))
#ifdef __cpp_exceptions
		throw std::system_error(std::make_error_code(std::errc::io_error));
#else
		fast_terminate();
#endif*/
	return begin+(t.rdb->sputn(static_cast<char_type const*>(static_cast<void const*>(std::to_address(begin))),(end-begin)*sizeof(*begin)/sizeof(char_type)));
}

template<std::integral CharT,typename Traits = std::char_traits<CharT>>
using basic_streambuf_io_observer = basic_general_streambuf_io_observer<std::basic_streambuf<CharT,Traits>>;

template<std::integral CharT,typename Traits = std::char_traits<CharT>>
using basic_filebuf_io_observer = basic_general_streambuf_io_observer<std::basic_filebuf<CharT,Traits>>;

template<std::integral CharT,typename Traits = std::char_traits<CharT>>
using basic_stringbuf_io_observer = basic_general_streambuf_io_observer<std::basic_stringbuf<CharT,Traits>>;


using streambuf_io_observer = basic_streambuf_io_observer<char>;
using wstreambuf_io_observer = basic_streambuf_io_observer<wchar_t>;
using u8streambuf_io_observer = basic_streambuf_io_observer<char8_t>;

using filebuf_io_observer = basic_filebuf_io_observer<char>;
using wfilebuf_io_observer = basic_filebuf_io_observer<wchar_t>;
using u8filebuf_io_observer = basic_filebuf_io_observer<char8_t>;

using stringbuf_io_observer = basic_stringbuf_io_observer<char>;
using wstringbuf_io_observer = basic_stringbuf_io_observer<wchar_t>;
using u8stringbuf_io_observer = basic_stringbuf_io_observer<char8_t>;

}


#if defined(__GLIBCXX__) || defined(__LIBCPP_VERSION) || defined(_MSVC_STL_UPDATE)
#include"general.h"
#endif
