#pragma once

namespace fast_io
{

inline constexpr char* ibuffer_begin(c_io_observer_unlocked cio) noexcept
{
	return cio.fp->_IO_read_base;
}

inline constexpr char* ibuffer_curr(c_io_observer_unlocked cio) noexcept
{
	return cio.fp->_IO_read_ptr;
}

inline constexpr char* ibuffer_end(c_io_observer_unlocked cio) noexcept
{
	return cio.fp->_IO_read_end;
}

inline constexpr void ibuffer_set_curr(c_io_observer_unlocked cio,char* ptr) noexcept
{
	cio.fp->_IO_read_ptr=ptr;
}

extern "C" int __underflow (FILE *) noexcept;
inline bool underflow(c_io_observer_unlocked cio) noexcept
{
	return __underflow(cio.fp)!=EOF;
}

inline constexpr char* obuffer_begin(c_io_observer_unlocked cio) noexcept
{
	return cio.fp->_IO_write_base;
}

inline constexpr char* obuffer_curr(c_io_observer_unlocked cio) noexcept
{
	return cio.fp->_IO_write_ptr;
}

inline constexpr char* obuffer_end(c_io_observer_unlocked cio) noexcept
{
	return cio.fp->_IO_write_end;
}

inline constexpr void obuffer_set_curr(c_io_observer_unlocked cio,char* ptr) noexcept
{
	cio.fp->_IO_write_ptr=ptr;
}

extern "C" int __overflow (FILE *,int) noexcept;

inline void overflow(c_io_observer_unlocked cio,char ch)
{
	if(__overflow(cio.fp,static_cast<int>(static_cast<unsigned char>(ch)))==EOF)[[unlikely]]
#ifdef __cpp_exceptions
		throw std::system_error(errno,std::generic_category());
#else
		fast_terminate();
#endif
}

}