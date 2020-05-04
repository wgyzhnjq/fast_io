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
void cxx_fast_io_release(void* deviceptr) CXX_FAST_IO_NOEXCEPT;

int cxx_fast_io_write(char const** written,void* device,char const* pointer_begin,char const* pointer_end) CXX_FAST_IO_NOEXCEPT;//[pointer_begin,pointer_end)

int cxx_fast_io_flush(void* device) CXX_FAST_IO_NOEXCEPT;//[pointer_begin,pointer_end)
*/

#ifdef __cplusplus
}

#endif

#undef CXX_FAST_IO_NOEXCEPT