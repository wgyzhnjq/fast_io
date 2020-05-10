#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"
#include<thread>

int main()
{
	fast_io::win32_file iocp(fast_io::io_async);
	std::vector<std::jthread> pools;
	pools.reserve(std::thread::hardware_concurrency());
	pools.emplace_back(fast_io::win32::single_thread_io_async_scheduling,iocp);
	fast_io::win32_file file(fast_io::io_async,iocp,"w.txt",fast_io::open_mode::out|fast_io::open_mode::binary|fast_io::open_mode::no_block);
	print("Hello World\n");
	std::string_view view("Hello World\n");
	async_write(file,view.cbegin(),view.cend(),[](std::size_t size)
	{
		println("transferred size:",size);
	});
}
