#pragma once
#include<stddef.h>
#include<errno.h>

//This library is stil header only. Just need compile for C interface if you want to use C interface

#ifdef __cplusplus
extern "C"
{
#define CXX_FAST_IO_NOEXCEPT noexcept
#else
#define CXX_FAST_IO_NOEXCEPT
#endif


//C++ does not define errno_t. Let's use int instead.
//no check for nullptr. precondition violation!
void cxx_fast_io_release(void* deviceptr) CXX_FAST_IO_NOEXCEPT;

int cxx_fast_io_writes(void* device,char const* pointer_begin,char const* pointer_end) CXX_FAST_IO_NOEXCEPT;//[pointer_begin,pointer_end)

int cxx_fast_io_flush(void* device) CXX_FAST_IO_NOEXCEPT;//[pointer_begin,pointer_end)
/*
TODO:
int cxx_fast_io_write_size(size_t* size,void* device) CXX_FAST_IO_NOEXCEPT;//[pointer_begin,pointer_end)

int cxx_fast_io_read_size(size_t* size,void* device) CXX_FAST_IO_NOEXCEPT;//[pointer_begin,pointer_end)
*/

int cxx_fast_io_reads(char** readed,void* device,char* pointer_begin,char* pointer_end) CXX_FAST_IO_NOEXCEPT;//[pointer_begin,pointer_end)

int cxx_fast_io_acquire_bufferred_file(void** generated_device,char const* filename,char const* open_mode) CXX_FAST_IO_NOEXCEPT;

int cxx_fast_io_acquire_file(void** generated_device,char const* filename,char const* open_mode) CXX_FAST_IO_NOEXCEPT;

int cxx_fast_io_acquire_stdin(void** generated_device) CXX_FAST_IO_NOEXCEPT;

int cxx_fast_io_acquire_stdout(void** generated_device) CXX_FAST_IO_NOEXCEPT;

int cxx_fast_io_acquire_stderr(void** generated_device) CXX_FAST_IO_NOEXCEPT;

int cxx_fast_io_print_size_t(void* device,size_t value) CXX_FAST_IO_NOEXCEPT;

int cxx_fast_io_print_ptrdiff_t(void* device,ptrdiff_t value) CXX_FAST_IO_NOEXCEPT;

int cxx_fast_io_print_double(void* device,double value) CXX_FAST_IO_NOEXCEPT;

int cxx_fast_io_print_c_str(void* device,char const* c_str) CXX_FAST_IO_NOEXCEPT;


#ifdef __cplusplus
}

#endif

#undef CXX_FAST_IO_NOEXCEPT