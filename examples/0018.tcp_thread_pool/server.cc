#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"
#include"../../include/fast_io_network.h"

int main()
{
	fast_io::tcp_server hd(2000);
	thread_pool_accept_callback<fast_io::acceptor>(hd,[](auto& acc)
	{
		print(acc,"Hello World\n");
	});
}