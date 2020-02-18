#include"../../include/fast_io_network.h"
#include"../../include/fast_io_device.h"
#include"../timer.h"

int main()
{
	fast_io::timer tm("fast_io tcp multithread");
	fast_io::obuf_file obf("client.txt");
	fast_io::basic_obuf<fast_io::tcp_client> hd(fast_io::ipv4{127,0,0,1},2000);
	for(std::size_t i{};i!=1;++i)
		println(hd,i);
	transmit(obf,hd);
}