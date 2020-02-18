#include"../../include/fast_io_network.h"
#include"../../include/fast_io_device.h"
#include"../timer.h"

int main()
{
	fast_io::timer tm("fast_io tcp multithread");
	fast_io::obuf_file obf("client.txt");
	fast_io::basic_obuf<fast_io::tcp_client> hd(fast_io::ipv4{192,168,0,54},2000);
	static_assert(fast_io::input_stream<fast_io::basic_obuf<fast_io::tcp_client>>);
/*	for(std::size_t i{};i!=10000000;++i)
		println(hd,i);
	transmit(obf,hd);*/
}