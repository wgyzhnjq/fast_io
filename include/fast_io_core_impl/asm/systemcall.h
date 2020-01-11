#pragma once
#include"asm_amd64.h"


namespace fast_io
{

template<std::integral I>
inline void fast_exit(I ret)
{
#if defined(__linux__) && defined(__x86_64__)
	fast_io::system_call_noreturn<60>(ret);
#else
	_exit(ret);
#endif
}

inline void fast_terminate()
{
	fast_exit(-1);
}
}