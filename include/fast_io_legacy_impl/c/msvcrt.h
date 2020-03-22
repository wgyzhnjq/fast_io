#pragma once

namespace fast_io
{

/*
mem layout
struct _iobuf
{
char *_ptr;
int _cnt;
char *_base;
int _flag;
int _file;
int _charbuf;
int _bufsiz;
char *_tmpfname;
};
*/
/*
namespace details
{

struct c_io_handle_curr_proxy
{
	std::FILE* fp={};
	using element_type = char*;
	inline constexpr c_io_handle_curr_proxy& operator=(char* ptr) noexcept
	{
		fp->_cnt-=ptr-fp->_ptr;
		fp->_ptr=ptr;
		return *this;
	}
	template<std::integral offset_type>
	inline constexpr c_io_handle_curr_proxy& operator+=(offset_type off) noexcept
	{
		fp->_ptr+=off;
		fp->_cnt-=off;
		return *this;
	}
	template<std::integral offset_type>
	inline constexpr c_io_handle_curr_proxy& operator-=(offset_type off) noexcept
	{
		fp->_ptr-=off;
		fp->_cnt+=off;
		return *this;
	}
	template<std::integral offset_type>
	inline constexpr auto& operator[](offset_type off) noexcept
	{
		return fp->_ptr[off];
	}
	template<std::integral offset_type>
	inline constexpr auto& operator[](offset_type off) const noexcept
	{
		return fp->_ptr[off];
	}
	inline constexpr auto operator->() noexcept
	{
		return fp->_ptr;
	}
	inline constexpr auto operator->() const noexcept
	{
		return fp->_ptr;
	}
	inline constexpr auto& operator*() noexcept
	{
		return *fp->_ptr;
	}
	inline constexpr auto& operator*() const noexcept
	{
		return *fp->_ptr;
	}
//	template<std::integral offset_type>
	inline constexpr auto& operator[](std::ptrdiff_t off) const noexcept
	{
		return fp->_ptr[off];
	}
//	template<std::integral offset_type>
	inline constexpr auto& operator[](std::ptrdiff_t off) noexcept
	{
		return fp->_ptr[off];
	}
	inline constexpr c_io_handle_curr_proxy& operator++() noexcept
	{
		++fp->_ptr;
		--fp->_cnt;
		return *this;
	}
	inline constexpr c_io_handle_curr_proxy& operator--() noexcept
	{
		--fp->_ptr;
		++fp->_cnt;
		return *this;
	}
	inline constexpr c_io_handle_curr_proxy operator++(int) noexcept
	{
		auto temp{*this};
		++*this;
		return temp;
	}
	inline constexpr c_io_handle_curr_proxy operator--(int) noexcept
	{
		auto temp{*this};
		--*this;
		return temp;
	}
};

inline constexpr bool operator==(char const * e,c_io_handle_curr_proxy io)
{
	return io.fp->_ptr==e;
}
inline constexpr bool operator!=(char const * e,c_io_handle_curr_proxy io)
{
	return io.fp->_ptr!=e;
}
inline constexpr bool operator==(c_io_handle_curr_proxy io,char const* e)
{
	return io.fp->_ptr==e;
}
inline constexpr bool operator!=(c_io_handle_curr_proxy io,char const * e)
{
	return io.fp->_ptr!=e;
}

}
*/
inline constexpr char* ibuffer_begin(c_io_observer_unlocked cio)
{
	return cio.fp->_base;
}

inline constexpr char* ibuffer_curr(c_io_observer_unlocked cio)
{
	return cio.fp->_ptr;
}

inline constexpr char* ibuffer_cend(c_io_observer_unlocked cio)
{
	return cio.fp->_ptr+cio.fp->_cnt;
}

inline constexpr void ibuffer_set_curr(c_io_observer_unlocked cio,char* ptr)
{
	cio.fp->_cnt-=ptr-cio.fp->_ptr;
	cio.fp->_ptr=ptr;
}

inline bool underflow(c_io_observer_unlocked cio)
{
	return _filbuf(cio.fp)!=EOF;
}

inline constexpr char* obuffer_cbegin(c_io_observer_unlocked cio)
{
	return cio.fp->_base;
}

inline constexpr char* obuffer_curr(c_io_observer_unlocked cio)
{
	return cio.fp->_ptr;
}

inline constexpr char* obuffer_cend(c_io_observer_unlocked cio)
{
	return cio.fp->_base+cio.fp->_bufsiz;
}

inline constexpr void obuffer_set_curr(c_io_observer_unlocked cio,char* ptr)
{
	cio.fp->_cnt-=ptr-cio.fp->_ptr;
	cio.fp->_ptr=ptr;
}

inline void overflow(c_io_observer_unlocked cio,char ch)
{
	cio.fp->_flag|=0x010000;
	if(_flsbuf(static_cast<int>(static_cast<unsigned char>(ch)),cio.fp)==EOF)[[unlikely]]
#ifdef __cpp_exceptions
		throw std::system_error(errno,std::generic_category());
#else
		fast_terminate();
#endif
}
/*
inline constexpr char* oreserve(c_io_observer_unlocked cio,std::size_t sz)
{
	if(cio.fp->_bufsiz-cio.fp->_cnt<sz)[[unlikely]]
		return nullptr;
	return cio.fp->_ptr;
}

inline constexpr void orelease(c_io_observer_unlocked cio,char* ptr)
{
	cio.fp->_cnt-=ptr-cio.fp->_ptr;
	cio.fp->_ptr=ptr;
}
*/

}