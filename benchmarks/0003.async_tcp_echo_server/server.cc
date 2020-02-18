#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"
#include"../../include/fast_io_network.h"

int main(int argc,char** argv)
{
	if(argc!=2)
	{
		print(fast_io::err,"Usage: ",*argv," <port>\n");
		return 1;
	}
	fast_io::tcp_server hd(fast_io::to<std::size_t>(argv[1]));
	thread_pool_accept<fast_io::acceptor>(hd,[](auto& acc)
	{
	try
	{
		transmit(acc,acc);
	}
	catch(...)
	{
	}
	});
}
