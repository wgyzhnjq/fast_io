#pragma once

#include"fast_io_hosted.h"

namespace fast_io
{
inline namespace
{
using in_type = fast_io::basic_iomutex<fast_io::basic_ibuf<native_io_handle>>;

in_type in(native_stdin_number);
}
}

template<bool report_eof=false,typename T,typename... Args>
requires (!fast_io::input_stream<std::remove_cvref_t<T>>)&&requires(T&& t,Args&& ...args)
{
	scan<report_eof>(fast_io::in,std::forward<T>(t),std::forward<Args>(args)...);
}
inline constexpr auto scan(T&& t,Args&& ...args)
{
	return scan<report_eof>(fast_io::in,std::forward<T>(t),std::forward<Args>(args)...);
}

#ifndef NDEBUG

template<bool report_eof=false,typename T,typename... Args>
requires (!fast_io::input_stream<std::remove_cvref_t<T>>)&&requires(T&& t,Args&& ...args)
{
	scan<report_eof>(fast_io::in,std::forward<T>(t),std::forward<Args>(args)...);
}
inline constexpr auto debug_scan(T&& t,Args&& ...args)
{
	return scan<report_eof>(fast_io::in,std::forward<T>(t),std::forward<Args>(args)...);
}

#endif