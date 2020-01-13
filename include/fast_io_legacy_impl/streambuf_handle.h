#pragma once
#include<streambuf>

#ifdef __GLIBCXX__
#include<ext/stdio_filebuf.h>
#endif

namespace fast_io
{

template<typename T>
class streambuf_handle
{
public:
	stream_view_details::streambuf_pub<T> handle;
	using streambuf_type = T;
	using char_type = typename streambuf_type::char_type;
	template<typename ...Args>
	requires std::constructible_from<T,Args...>
	streambuf_handle(Args&& ...args):handle(std::forward<Args>(args)...)
	{
	}
	auto& native_handle()
	{
		return handle;
	}

};

template<typename T,std::contiguous_iterator Iter>
inline Iter read(streambuf_handle<T>& hd,Iter begin,Iter end)
{
	using char_type = typename T::char_type;
	return begin+(hd.handle.sgetn(static_cast<char_type*>(static_cast<void*>(std::to_address(begin))),(end-begin)*sizeof(*begin)/sizeof(char_type))*sizeof(char_type)/sizeof(*begin));
}

template<typename T,std::contiguous_iterator Iter>
inline void write(streambuf_handle<T>& hd,Iter begin,Iter end) 
{
	using char_type = typename T::char_type;
	if(!hd.handle.sputn(static_cast<char_type const*>(static_cast<void const*>(std::to_address(begin))),(end-begin)*sizeof(*begin)/sizeof(char_type)))
#ifdef __cpp_exceptions
		throw std::system_error(std::make_error_code(std::errc::io_error));
#else
		fast_terminate();
#endif
}

template<typename T>
inline void flush(streambuf_handle<T>& hd)
{
	hd.handle.flush();
}

template<typename T>
inline auto oreserve(streambuf_handle<T>& hd,std::size_t size) -> typename T::char_type*
{
	auto p(hd.handle.pptr());
	if(hd.handle.epptr()-p<=size)
		return nullptr;
	hd.handle.pbump(static_cast<int>(size));
	return p+size;
}

template<typename T>
inline void orelease(streambuf_handle<T>& hd,std::size_t size)
{
	hd.handle.pbump(-static_cast<int>(size));
}
#ifdef __GLIBCXX__


template<std::integral ch_type>
class basic_filebuf_handle:public streambuf_handle<__gnu_cxx::stdio_filebuf<ch_type>>
{
public:
	using native_handle_type = __gnu_cxx::stdio_filebuf<ch_type>;
	using char_type = ch_type;
	template<typename ...Args>
	requires std::constructible_from<streambuf_handle<__gnu_cxx::stdio_filebuf<ch_type>>,int,Args...>
	basic_filebuf_handle(basic_posix_io_handle<ch_type>& hd,Args&& ...args):
		streambuf_handle<__gnu_cxx::stdio_filebuf<ch_type>>(hd.native_handle(),std::forward<Args>(args)...)
	{}
	template<typename ...Args>
	requires std::constructible_from<streambuf_handle<__gnu_cxx::stdio_filebuf<ch_type>>,int,Args...>
	basic_filebuf_handle(basic_c_style_io_handle<ch_type>& hd,Args&& ...args):
		streambuf_handle<__gnu_cxx::stdio_filebuf<ch_type>>(hd.native_handle(),std::forward<Args>(args)...)
	{}

	template<typename ...Args>
	requires std::constructible_from<streambuf_handle<__gnu_cxx::stdio_filebuf<ch_type>>,int,Args...>
	basic_filebuf_handle(basic_c_style_io_handle_unlocked<ch_type>& hd,Args&& ...args):
		streambuf_handle<__gnu_cxx::stdio_filebuf<ch_type>>(hd.native_handle(),std::forward<Args>(args)...)
	{}
#ifdef __WINNT__
	explicit operator basic_win32_io_handle<char_type>()
	{
		return _get_osfhandle(this->handle.fd());
	}
#endif
	explicit operator basic_posix_io_handle<char_type>()
	{
		return this->handle.fd();
	}

	explicit operator basic_c_style_io_handle_unlocked<char_type>()
	{
		return this->handle.file();
	}

	explicit operator basic_c_style_io_handle<char_type>()
	{
		return this->handle.file();
	}
};

using filebuf_handle = basic_filebuf_handle<char>;

#ifdef __linux__
template<typename T>
inline auto zero_copy_in_handle(basic_filebuf_handle<T>& h)
{
	return h.handle.fd();
}

template<typename T>
inline auto zero_copy_out_handle(basic_filebuf_handle<T>& h)
{
	return h.handle.fd();
}
#endif


#endif
}