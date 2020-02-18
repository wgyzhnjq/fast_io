#include"../../include/fast_io_network.h"
#include"../../include/fast_io_device.h"
#include"../timer.h"

int main()
{
	fast_io::timer tm("fast_io tcp multithread");
	for(std::size_t i{};i!=1000;++i)
	{
		fast_io::tcp_client hd(fast_io::ipv4{192,168,0,54},2000);
	}
}