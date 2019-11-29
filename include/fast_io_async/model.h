#pragma once

namespace fast_io
{

enum class async_model
{
	unknown = 0,
	iocp = 1,
	epoll = 2,
	kqueue = 3,
	native =
#if defined(__WINNT__) || defined(_MSC_VER)
	iocp
#elif defined(__linux__)
	epoll
#elif defined(__FreeBSD__)
	kqueue
#else
	unknown
#endif
};

}