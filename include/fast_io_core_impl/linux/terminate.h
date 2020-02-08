#pragma once

namespace fast_io
{

template<std::integral I>
[[noreturn]] inline void fast_exit(I ret)
{
#if defined(__linux__) && defined(__x86_64__)
	system_call_no_return<60>(ret);
#else
	_exit(ret);
#endif
}

[[noreturn]] inline void fast_terminate()
{
	fast_exit(-1);
}

}