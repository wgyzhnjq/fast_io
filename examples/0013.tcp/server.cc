#include"../../include/fast_io.h"
#include"../../include/fast_io_network.h"

int main()
try
{
	fast_io::tcp_server hd(2000);
	for(;;)
	{
		fast_io::acceptor accept(hd);
		print(accept,u8"hello world from server\n");
	}
}
catch(std::exception const & e)
{
	println(fast_io::err,e);
	return 1;
}