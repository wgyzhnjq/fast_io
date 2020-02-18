#include"../../include/fast_io_network.h"
#include"../../include/fast_io_device.h"
#include"../../include/fast_io.h"

int main(int argc,char** argv)
{
	if(argc!=2)
	{
		print(fast_io::err,"Usage: ",*argv," <port>\n");
		return 1;
	}
	std::size_t port(fast_io::to<std::size_t>(argv[1]));
	std::vector<std::jthread> jth;	
	for(std::size_t i{};i!=1000;++i)
		jth.emplace_back([port]()
		{
			fast_io::tcp_client hd(fast_io::ipv4{127,0,0,1},port);
			fast_io::transmit(hd,hd);
		});
}