#pragma once

namespace fast_io
{
/*

referenced from win10sdk ucrt
C:\Program Files (x86)\Windows Kits\10\Source\10.0.19041.0\ucrt

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//
// Internal Stream Types (__crt_stdio_stream and friends)
//
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
struct __crt_stdio_stream_data
{
    union
    {
        FILE  _public_file;
        char* _ptr;
    };

    char*            _base;
    int              _cnt;
    long             _flags;
    long             _file;
    int              _charbuf;
    int              _bufsiz;
    char*            _tmpfname;
    CRITICAL_SECTION _lock;
};

// Ensure that __crt_stdio_stream_data* and FILE* pointers are freely convertible:
*/


inline char* ibuffer_begin(c_io_observer_unlocked cio)
{
	return reinterpret_cast<__crt_stdio_stream_data*>(cio.fp)->_base;
}

inline char* ibuffer_curr(c_io_observer_unlocked cio)
{
	return reinterpret_cast<__crt_stdio_stream_data*>(cio.fp)->_ptr;
}

inline char* ibuffer_end(c_io_observer_unlocked cio)
{
	return reinterpret_cast<__crt_stdio_stream_data*>(cio.fp)->_ptr+reinterpret_cast<__crt_stdio_stream_data*>(cio.fp)->_cnt;
}

inline constexpr void ibuffer_set_curr(c_io_observer_unlocked cio,char* ptr)
{
	reinterpret_cast<__crt_stdio_stream_data*>(cio.fp)->_cnt-=ptr-reinterpret_cast<__crt_stdio_stream_data*>(cio.fp)->_ptr;
	reinterpret_cast<__crt_stdio_stream_data*>(cio.fp)->_ptr=ptr;
}

inline bool underflow(c_io_observer_unlocked cio)
{
	if(_filbuf(reinterpret_cast<__crt_stdio_stream_data*>(cio.fp))==EOF)[[unlikely]]
		return false;
	++reinterpret_cast<__crt_stdio_stream_data*>(cio.fp)->_cnt;
	--reinterpret_cast<__crt_stdio_stream_data*>(cio.fp)->_ptr;
	return true;
}

inline char* obuffer_begin(c_io_observer_unlocked cio)
{
	return reinterpret_cast<__crt_stdio_stream_data*>(cio.fp)->_base;
}

inline char* obuffer_curr(c_io_observer_unlocked cio)
{
	return reinterpret_cast<__crt_stdio_stream_data*>(cio.fp)->_ptr;
}

inline char* obuffer_end(c_io_observer_unlocked cio)
{
	return reinterpret_cast<__crt_stdio_stream_data*>(cio.fp)->_base+reinterpret_cast<__crt_stdio_stream_data*>(cio.fp)->_bufsiz;
}

inline void obuffer_set_curr(c_io_observer_unlocked cio,char* ptr)
{
	reinterpret_cast<__crt_stdio_stream_data*>(cio.fp)->_cnt-=ptr-reinterpret_cast<__crt_stdio_stream_data*>(cio.fp)->_ptr;
	reinterpret_cast<__crt_stdio_stream_data*>(cio.fp)->_ptr=ptr;
}

inline void overflow(c_io_observer_unlocked cio,char ch)
{
	reinterpret_cast<__crt_stdio_stream_data*>(cio.fp)->_flag|=0x010000;
	if(_flsbuf(static_cast<int>(static_cast<unsigned char>(ch)),reinterpret_cast<__crt_stdio_stream_data*>(cio.fp))==EOF)[[unlikely]]
#ifdef __cpp_exceptions
		throw std::system_error(errno,std::generic_category());
#else
		fast_terminate();
#endif
}

}