#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"
#include"../../include/fast_io_network.h"

fast_io::task listener(fast_io::tcp_server& server)
{
	auto& acceptor(co_await fast_io::basic_thread_pool_awaitable<fast_io::tcp_server,fast_io::acceptor>(server));
}

int main()
{
	fast_io::tcp_server hd(2000);
	listener(hd);
}