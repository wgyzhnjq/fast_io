#include"../../include/fast_io_network.h"
#include"../../include/fast_io_device.h"
#include"../timer.h"

int main()
{
	fast_io::timer tm("fast_io tcp multithread");
	fast_io::obuf_file obf("client.txt");
	for(std::size_t i{};i!=1000;++i)
	{
		fast_io::tcp_client hd(fast_io::ipv4{127,0,0,1},2000);
		transmit(obf,hd);
	}
}