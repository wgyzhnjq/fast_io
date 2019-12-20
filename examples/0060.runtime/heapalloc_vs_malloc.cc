#include"timer.h"
#include<windows.h>
#include<exception>
/*
[[nodiscard]] inline void* operator new(std::size_t bytes) noexcept
{
	auto allocated(HeapAlloc(GetProcessHeap(),0,bytes));
	if(allocated==nullptr) [[unlikely]]
		std::terminate();
	return allocated;
}

[[nodiscard]] inline void* operator new[](std::size_t bytes) noexcept
{
	return ::operator new(bytes);
}

[[nodiscard]] inline void* operator new(std::size_t bytes,std::nothrow_t const&) noexcept
{
	return HeapAlloc(GetProcessHeap(),0,bytes);
}

[[nodiscard]] inline void* operator new[](std::size_t bytes,std::nothrow_t const&) noexcept
{
	return HeapAlloc(GetProcessHeap(),0,bytes);
}

[[nodiscard]] inline void* operator new(std::size_t count, void* ptr) noexcept
{
	auto allocated(HeapReAlloc(GetProcessHeap(),0,ptr,count));
	if(allocated==nullptr) [[unlikely]]
		std::terminate();
	return allocated;
}

[[nodiscard]] inline void* operator new[](std::size_t count, void* ptr) noexcept
{
	return ::operator new(count,ptr);
}
*/

int main()
{
	std::set_terminate([]()
	{
		ExitProcess(-1);
	});
	constexpr std::size_t bytes(10);
	constexpr std::size_t loops(10000000);
	std::size_t sm(0);
	{
	cqw::timer tm("HeapAlloc/HeapFree");
//	auto hp(GetProcessHeap());
//	HeapUnlock(hp);
	for(std::size_t i(0);i!=loops;++i)
	{
		auto v(HeapAlloc(GetProcessHeap(),0,bytes));
		sm+=fast_io::bit_cast<std::size_t>(v);
		HeapFree(GetProcessHeap(),0,v);
	}
//	HeapLock(hp);
	}
	{
	cqw::timer tm("malloc/free");
	for(std::size_t i(0);i!=loops;++i)
	{
		auto v(malloc(bytes));
		sm+=fast_io::bit_cast<std::size_t>(v);
		free(v);
	}
	}
	println(fast_io::out,sm);
}