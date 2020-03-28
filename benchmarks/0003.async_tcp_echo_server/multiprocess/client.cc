#include"../../timer.h"
#include"../../../include/fast_io.h"
#include"../../../include/fast_io_network.h"

int main()
{
try
{
	fast_io::timer tm("total");
	fast_io::tcp_client hd(fast_io::ipv4{118,25,105,254},2000);
	for(std::size_t i{};i!=10;++i)
	{
		fast_io::timer tm1("some");
		print(hd,"Hello World\n");
		transmit_once(fast_io::out,hd);
	}
}
catch(std::exception const& e)
{
	println(fast_io::err,e);
	return 1;
}
}