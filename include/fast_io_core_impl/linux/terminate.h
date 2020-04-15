#pragma once

namespace fast_io
{

template<std::integral I>
inline void fast_exit(I ret)
{
#if defined(__linux__) && defined(__x86_64__)
	system_call_no_return<60>(ret);
#else
	_exit(ret);
#endif
}

inline void fast_terminate()
{
	*(reinterpret_cast<char*>(0))=0;
	//create a null pointer hardware exception to terminate the program
//	fast_exit(-1);
}

}