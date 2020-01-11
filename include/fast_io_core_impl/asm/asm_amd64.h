#pragma once

namespace fast_io
{
//#if defined(__GNU__) || defined(__clang__)

#ifndef _MSC_VER

#ifdef __x86_64__
template<std::size_t syscall_number,typename return_value_type>
inline auto system_call(auto p1, auto p2, auto p3)
{
	return_value_type ret;
	__asm__ __volatile__
	(
	"syscall"
	: "=a" (ret)
	//EDI      RSI       RDX
	: "0"(syscall_number), "D"(p1), "S"(p2), "d"(p3)
	: "rcx", "r11", "memory"
	);
	return ret;
}

template<std::size_t syscall_number>
inline void system_call_noreturn(auto p1)
{
	std::size_t ret;
	__asm__ __volatile__("syscall": "=a" (ret): "0"(syscall_number), "D"(p1):"rcx", "r11", "memory");
}
#endif

#endif

}