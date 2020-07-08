#pragma once

#if defined(__linux__)
#ifdef __x86_64__
#include"amd64.h"
#endif
#if defined(__arm64__) || defined(__aarch64__) 
#include "aarch64.h"
#endif
#endif


namespace fast_io
{

template<std::integral I>
requires(sizeof(I)>=1)
inline void system_call_throw_error(I v)
{
#if defined(__linux__) && (defined(__x86_64__) || defined(__arm64__) || defined(__aarch64__) )
	using unsigned_t = std::make_unsigned_t<I>;
	if(static_cast<unsigned_t>(v)+static_cast<unsigned_t>(4096)<static_cast<unsigned_t>(4096))
		FIO_POSIX_ERROR(static_cast<int>(-v));
#else
	if(v<0)
		FIO_POSIX_ERROR();
#endif
}

}