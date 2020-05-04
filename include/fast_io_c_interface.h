#pragma once
#include<stddef.h>

//This library is stil header only. Just need compile for C interface if you want to use C interface

#ifdef __cplusplus
extern "C"
{
#define CXX_FAST_IO_NOEXCEPT noexcept
#else
#define CXX_FAST_IO_NOEXCEPT
#endif
/*
//C++ does not define errno_t. Let's use int instead.
//no check for nullptr. precondition violation!
*/

[[nodiscard("Please NEVER ignore the error code. Ignoring error codes are 99% sources of the security vulnerabilities of C programs.")]] int cxx_fast_io_read(char** readed,void* d,char* begin,char* end) CXX_FAST_IO_NOEXCEPT;

[[nodiscard("Please NEVER ignore the error code. Ignoring error codes are 99% sources of the security vulnerabilities of C programs.")]] int cxx_fast_io_write(char const** written,void* d,char const* begin,char const* end) CXX_FAST_IO_NOEXCEPT;

[[nodiscard("Please NEVER ignore the error code. Ignoring error codes are 99% sources of the security vulnerabilities of C programs.")]] int cxx_fast_io_flush(void* d) CXX_FAST_IO_NOEXCEPT;

[[nodiscard("Please NEVER ignore the error code. Ignoring error codes are 99% sources of the security vulnerabilities of C programs.")]] int cxx_fast_io_print_size_t(void* d,std::size_t value) CXX_FAST_IO_NOEXCEPT;

[[nodiscard("Please NEVER ignore the error code. Ignoring error codes are 99% sources of the security vulnerabilities of C programs.")]] int cxx_fast_io_print_ptrdiff_t(void* d,std::ptrdiff_t value) CXX_FAST_IO_NOEXCEPT;

[[nodiscard("Please NEVER ignore the error code. Ignoring error codes are 99% sources of the security vulnerabilities of C programs.")]] int cxx_fast_io_print_double(void* d,double value) CXX_FAST_IO_NOEXCEPT;

[[nodiscard("Please NEVER ignore the error code. Ignoring error codes are 99% sources of the security vulnerabilities of C programs.")]] int cxx_fast_io_println_size_t(void* d,std::size_t value) CXX_FAST_IO_NOEXCEPT;

[[nodiscard("Please NEVER ignore the error code. Ignoring error codes are 99% sources of the security vulnerabilities of C programs.")]] int cxx_fast_io_println_ptrdiff_t(void* d,std::ptrdiff_t value) CXX_FAST_IO_NOEXCEPT;

[[nodiscard("Please NEVER ignore the error code. Ignoring error codes are 99% sources of the security vulnerabilities of C programs.")]] int cxx_fast_io_println_double(void* d,double value) CXX_FAST_IO_NOEXCEPT;

[[nodiscard("Please NEVER ignore the error code. Ignoring error codes are 99% sources of the security vulnerabilities of C programs.")]] int cxx_fast_io_bufferred_read(char** readed,void* d,char* begin,char* end) CXX_FAST_IO_NOEXCEPT;

[[nodiscard("Please NEVER ignore the error code. Ignoring error codes are 99% sources of the security vulnerabilities of C programs.")]] int cxx_fast_io_bufferred_write(char const** written,void* d,char const* begin,char const* end) CXX_FAST_IO_NOEXCEPT;

[[nodiscard("Please NEVER ignore the error code. Ignoring error codes are 99% sources of the security vulnerabilities of C programs.")]] int cxx_fast_io_bufferred_flush(void* d) CXX_FAST_IO_NOEXCEPT;

[[nodiscard("Please NEVER ignore the error code. Ignoring error codes are 99% sources of the security vulnerabilities of C programs.")]] int cxx_fast_io_bufferred_print_size_t(void* d,std::size_t value) CXX_FAST_IO_NOEXCEPT;

[[nodiscard("Please NEVER ignore the error code. Ignoring error codes are 99% sources of the security vulnerabilities of C programs.")]] int cxx_fast_io_bufferred_print_ptrdiff_t(void* d,std::ptrdiff_t value) CXX_FAST_IO_NOEXCEPT;

[[nodiscard("Please NEVER ignore the error code. Ignoring error codes are 99% sources of the security vulnerabilities of C programs.")]] int cxx_fast_io_bufferred_print_double(void* d,double value) CXX_FAST_IO_NOEXCEPT;

[[nodiscard("Please NEVER ignore the error code. Ignoring error codes are 99% sources of the security vulnerabilities of C programs.")]] int cxx_fast_io_bufferred_println_size_t(void* d,std::size_t value) CXX_FAST_IO_NOEXCEPT;

[[nodiscard("Please NEVER ignore the error code. Ignoring error codes are 99% sources of the security vulnerabilities of C programs.")]] int cxx_fast_io_bufferred_println_ptrdiff_t(void* d,std::ptrdiff_t value) CXX_FAST_IO_NOEXCEPT;

[[nodiscard("Please NEVER ignore the error code. Ignoring error codes are 99% sources of the security vulnerabilities of C programs.")]] int cxx_fast_io_bufferred_println_double(void* d,double value) CXX_FAST_IO_NOEXCEPT;

[[nodiscard("Please NEVER ignore the error code. Ignoring error codes are 99% sources of the security vulnerabilities of C programs.")]] int cxx_fast_io_bufferred_scan_size_t(void* d,std::size_t* value) CXX_FAST_IO_NOEXCEPT;

[[nodiscard("Please NEVER ignore the error code. Ignoring error codes are 99% sources of the security vulnerabilities of C programs.")]] int cxx_fast_io_bufferred_scan_ptrdiff_t(void* d,std::ptrdiff_t* value) CXX_FAST_IO_NOEXCEPT;

[[nodiscard("Please NEVER ignore the error code. Ignoring error codes are 99% sources of the security vulnerabilities of C programs.")]] int cxx_fast_io_bufferred_scan_double(void* d,double* value) CXX_FAST_IO_NOEXCEPT;

#if defined(__WINNT__) || defined(_MSC_VER)
[[nodiscard("Please NEVER ignore the error code. Ignoring error codes are 99% sources of the security vulnerabilities of C programs.")]] int cxx_fast_io_win32_handle_open(void** gen,void* handle,bool noclose,bool buffered) CXX_FAST_IO_NOEXCEPT;
#endif
[[nodiscard("Please NEVER ignore the error code. Ignoring error codes are 99% sources of the security vulnerabilities of C programs.")]] int cxx_fast_io_posix_fd_open(void** gen,int fd,bool noclose,bool buffered) CXX_FAST_IO_NOEXCEPT;

[[nodiscard("Please NEVER ignore the error code. Ignoring error codes are 99% sources of the security vulnerabilities of C programs.")]] int cxx_fast_io_c_fp_open(void** gen,std::FILE* fp,bool noclose,bool buffered) CXX_FAST_IO_NOEXCEPT;

void cxx_fast_io_release(void* d,bool buffered) CXX_FAST_IO_NOEXCEPT;

void cxx_fast_io_buffered_get_opointers(void* d,char*** begin,char*** curr,char*** end) CXX_FAST_IO_NOEXCEPT;

[[nodiscard("Please NEVER ignore the error code. Ignoring error codes are 99% sources of the security vulnerabilities of C programs.")]] int cxx_fast_io_buffered_overflow(void* d,char ch) CXX_FAST_IO_NOEXCEPT;

[[nodiscard("Please NEVER ignore the error code. Ignoring error codes are 99% sources of the security vulnerabilities of C programs.")]] int cxx_fast_io_buffered_read(char** readed,void* d,char* begin,char* end) CXX_FAST_IO_NOEXCEPT;

[[nodiscard("Please NEVER ignore the error code. Ignoring error codes are 99% sources of the security vulnerabilities of C programs.")]] int cxx_fast_io_buffered_write(char const** written,void* d,char const* begin,char const* end) CXX_FAST_IO_NOEXCEPT;

[[nodiscard("Please NEVER ignore the error code. Ignoring error codes are 99% sources of the security vulnerabilities of C programs.")]] int cxx_fast_io_buffered_flush(void* d) CXX_FAST_IO_NOEXCEPT;

void cxx_fast_io_buffered_get_ipointers(void* d,char*** begin,char*** curr,char*** end) CXX_FAST_IO_NOEXCEPT;

[[nodiscard("Please NEVER ignore the error code. Ignoring error codes are 99% sources of the security vulnerabilities of C programs.")]] int cxx_fast_io_buffered_underflow(char *ch,void* d) CXX_FAST_IO_NOEXCEPT;

#ifdef __cplusplus
}

#endif

#undef CXX_FAST_IO_NOEXCEPT