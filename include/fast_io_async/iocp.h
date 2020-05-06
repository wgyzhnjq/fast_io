#pragma once
#include<thread>

namespace fast_io::win32
{

extern "C" void* __stdcall CreateIoCompletionPort(void*,void*,std::uintptr_t,std::uint32_t);

template<typename... Args>
requires (sizeof...(Args)==4)
inline auto create_io_completion_port(Args&& ...args)
{
	auto ptr{CreateIoCompletionPort(std::forward<Args>(args)...)};
	if(ptr==nullptr)[[unlikely]]
		throw win32_error();
	return ptr;
}
/*
template<typename Func>
inline void worker_thread(Func f,win32_io_observer iocp)
{

}
*/
/*
template<typename func>
inline void iocp_spawn(func f)
{
	std::size_t thread_num(std::thread::hardware_concurrency()<<1);
	if(thread_num==0)[[unlikely]]
		thread_num = 2;
	std::vector<std::jthread> jthreads;
	jthreads.reserve(thread_num);
	std::vector<win32_file> iocps;
	iocps.reserve(thread_num);
	win32_file iocp_handle(create_io_completion_port(void*,void*,0,currency<<1));
	for(std::size_t i{};i!=thread_num;++i)
		jthreads.emplace_back(f,iocp_handle);
	for(;;)
	{
		win32_file iob(f());
		win32_file iocp(create_io_completion_port(iob.native_handle(),iocp_handle.native_handle(),0,currency<<1));
	}
}
*/


}