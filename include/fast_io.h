#pragma once

#include"fast_io_hosted.h"

#include"fast_io_legacy_impl/c/impl.h"

namespace fast_io
{

inline c_io_observer c_stdin() noexcept
{
	return {stdin};
}

inline c_io_observer c_stdout() noexcept
{
	return {stdout};
}

inline c_io_observer c_stderr() noexcept
{
	return {stderr};
}

inline wc_io_observer wc_stdin() noexcept
{
	return {stdin};
}

inline wc_io_observer wc_stdout() noexcept
{
	return {stdout};
}

inline wc_io_observer wc_stderr() noexcept
{
	return {stderr};
}

inline
#if !defined(__WINNT__) && !defined(_MSC_VER)
constexpr
#endif
decltype(auto) in() noexcept
{
	return native_stdin();
}

inline 
#if !defined(__WINNT__) && !defined(_MSC_VER)
constexpr
#endif
decltype(auto) out() noexcept
{
	return native_stdout();
}

inline
#if !defined(__WINNT__) && !defined(_MSC_VER)
constexpr
#endif
decltype(auto) err() noexcept
{
	return native_stderr();
}

using in_buf_type = basic_ibuf<native_io_observer>;
using out_buf_type = basic_obuf<native_io_observer>;

inline auto in_buf()
{
	return in_buf_type(native_stdin());
}

inline auto out_buf()
{
	return out_buf_type(native_stdout());
}

inline auto err_buf()
{
	return out_buf_type(native_stderr());
}

}

template<typename T,typename... Args>
inline constexpr void print(T&& t,Args&& ...args)
{
	if constexpr(fast_io::output_stream<std::remove_cvref_t<T>>)
		fast_io::print(std::forward<T>(t),std::forward<Args>(args)...);
	else
		fast_io::print(fast_io::c_io_observer{stdout},std::forward<T>(t),std::forward<Args>(args)...);
}

template<typename T,typename... Args>
inline constexpr void println(T&& t,Args&& ...args)
{
	if constexpr(fast_io::output_stream<std::remove_cvref_t<T>>)
		fast_io::println(std::forward<T>(t),std::forward<Args>(args)...);
	else
		fast_io::println(fast_io::c_io_observer{stdout},std::forward<T>(t),std::forward<Args>(args)...);
}

template<typename... Args>
inline constexpr void print_err(Args&&... args)
{
	fast_io::print(fast_io::err(),std::forward<Args>(args)...);
}

template<typename... Args>
inline constexpr void println_err(Args&&... args)
{
	fast_io::println(fast_io::err(),std::forward<Args>(args)...);
}


//Allow debug print
#ifndef NDEBUG

template<typename... Args>
inline constexpr void debug_print(Args&& ...args)
{
	::print(std::forward<Args>(args)...);
}

template<typename... Args>
inline constexpr void debug_println(Args&& ...args)
{
	::println(std::forward<Args>(args)...);
}

template<typename... Args>
inline constexpr void debug_print_err(Args&&... args)
{
	::print_err(std::forward<Args>(args)...);
}

template<typename... Args>
inline constexpr void debug_println_err(Args&&... args)
{
	::println_err(std::forward<Args>(args)...);
}
#endif

template<bool report_eof=false,typename T,typename... Args>
inline constexpr auto scan(T&& t,Args&& ...args)
{
	if constexpr(fast_io::input_stream<std::remove_cvref_t<T>>)
		return fast_io::scan<report_eof>(std::forward<T>(t),std::forward<Args>(args)...);
	else
		return scan<report_eof>(fast_io::c_io_observer{stdin},std::forward<T>(t),std::forward<Args>(args)...);
}
