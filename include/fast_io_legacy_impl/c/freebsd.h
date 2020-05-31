#pragma once
//https://github.com/freebsd/freebsd/blob/master/include/stdio.h


namespace fast_io
{

inline constexpr char* obuffer_begin(c_io_observer_unlocked cio) noexcept
{
	return reinterpret_cast<char*>(cio.fp->_bf.base);
}

inline constexpr char* obuffer_curr(c_io_observer_unlocked cio) noexcept
{
	return reinterpret_cast<char*>(cio.fp->_p);
}

inline constexpr char* obuffer_end(c_io_observer_unlocked cio) noexcept
{
	return reinterpret_cast<char*>(cio.fp->_bf._p+cio.fp->_bf._w);
}

inline constexpr void obuffer_set_curr(c_io_observer_unlocked cio,char* ptr) noexcept
{
	cio.fp->_w-=reinterpret_cast<char unsigned*>(ptr)-cio.fp->_p;
	cio.fp->_p=reinterpret_cast<char unsigned*>(ptr);
}

extern "C" int __overflow (FILE *,int) noexcept;

inline void overflow(c_io_observer_unlocked cio,char ch)
{
	obuffer_set_curr(cio,obuffer_end(cio));
	if(fputc(static_cast<int>(static_cast<char unsigned>(ch)),cio.fp)==EOF)[[unlikely]]
#ifdef __cpp_exceptions
		throw posix_error();
#else
		fast_terminate();
#endif
}


}