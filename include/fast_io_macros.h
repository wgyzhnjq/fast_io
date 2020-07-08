#pragma once

#if __cpp_constexpr >= 201907L
    #define FIO_CONSTEXPR constexpr
#else
    #define FIO_CONSTEXPR
#endif

#if __cpp_constexpr_dynamic_alloc >= 201907L
    #define FIO_DYN_CONSTEXPR constexpr
#else
    #define FIO_DYN_CONSTEXPR
#endif

#ifdef __cpp_exceptions
    #define FIO_POSIX_ERROR(e) throw posix_error(e)
#else
    #define FIO_POSIX_ERROR(e) fast_terminate()
#endif

#ifdef __cpp_exceptions
    #define FIO_WIN32_ERROR(e) throw win32_error(e)
#else
	#define FIO_WIN32_ERROR(e) fast_terminate()
#endif

#ifdef __cpp_exceptions
	#define FIO_TEXT_ERROR(e) throw fast_io_text_error(e)
#else
	#define FIO_TEXT_ERROR(e) fast_terminate()
#endif

#ifdef __cpp_exceptions
	#define FIO_EOF() throw eof()
#else
	#define FIO_EOF() fast_terminate();
#endif

#ifdef __cpp_exceptions
	#define FIO_OPENSSL_ERROR(e) throw openssl_error(e)
#else
	#define FIO_OPENSSL_ERROR(e) fast_terminate()
#endif

#ifdef __cpp_exceptions
	#define FIO_NT_ERROR(e) throw nt_error(e)
#else
	#define FIO_NT_ERROR(e) fast_terminate()
#endif
