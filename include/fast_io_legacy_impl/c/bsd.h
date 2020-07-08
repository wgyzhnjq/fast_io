#pragma once
//https://github.com/freebsd/freebsd/blob/master/include/stdio.h


namespace fast_io
{

inline char* obuffer_begin(c_io_observer_unlocked cio) noexcept
{
	return reinterpret_cast<char*>(cio.fp->_bf._base);
}

inline char* obuffer_curr(c_io_observer_unlocked cio) noexcept
{
	return reinterpret_cast<char*>(cio.fp->_p);
}

inline char* obuffer_end(c_io_observer_unlocked cio) noexcept
{
	return reinterpret_cast<char*>(cio.fp->_p+cio.fp->_w);
}

inline void obuffer_set_curr(c_io_observer_unlocked cio,char* ptr) noexcept
{
	cio.fp->_w-=reinterpret_cast<char unsigned*>(ptr)-cio.fp->_p;
	cio.fp->_p=reinterpret_cast<char unsigned*>(ptr);
}

inline void overflow(c_io_observer_unlocked cio,char ch)
{
	obuffer_set_curr(cio,obuffer_end(cio));
	if(__sputc(static_cast<int>(static_cast<char unsigned>(ch)),cio.fp)==EOF)[[unlikely]]
		FIO_POSIX_ERROR();
}

inline char* ibuffer_begin(c_io_observer_unlocked cio) noexcept
{
	return reinterpret_cast<char*>(cio.fp->_bf._base);
}

inline char* ibuffer_curr(c_io_observer_unlocked cio) noexcept
{
	return reinterpret_cast<char*>(cio.fp->_p);
}

inline char* ibuffer_end(c_io_observer_unlocked cio) noexcept
{
	return reinterpret_cast<char*>(cio.fp->_p+cio.fp->_r);
}

inline void ibuffer_set_curr(c_io_observer_unlocked cio,char* ptr) noexcept
{
	cio.fp->_r-=reinterpret_cast<char unsigned*>(ptr)-cio.fp->_p;
	cio.fp->_p=reinterpret_cast<char unsigned*>(ptr);
}

extern "C" int	__srget(FILE *);

inline bool underflow(c_io_observer_unlocked cio)
{
	bool eof{__srget(cio.fp)!=EOF};
	if(!eof&&ferror_unlocked(cio.fp))
		FIO_POSIX_ERROR();
	ibuffer_set_curr(cio,ibuffer_begin(cio));
	return eof;
}

}