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

inline constexpr char* ibuffer_begin(c_io_observer_unlocked cio)
{
	return cio.fp->_base;
}

inline constexpr char* ibuffer_curr(c_io_observer_unlocked cio)
{
	return cio.fp->_ptr;
}

inline constexpr char* ibuffer_end(c_io_observer_unlocked cio)
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
	if(_filbuf(cio.fp)==EOF)[[unlikely]]
		return false;
	++cio.fp->_cnt;
	--cio.fp->_ptr;
	return true;
}

inline constexpr char* obuffer_begin(c_io_observer_unlocked cio)
{
	return cio.fp->_base;
}

inline constexpr char* obuffer_curr(c_io_observer_unlocked cio)
{
	return cio.fp->_ptr;
}

inline constexpr char* obuffer_end(c_io_observer_unlocked cio)
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

}