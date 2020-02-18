#include"../../include/fast_io_network.h"
#include"../../include/fast_io_device.h"

int main()
{
	//impossible to stop
	fast_io::basic_obuf<fast_io::tcp_client> hd(fast_io::ipv4{127,0,0,1},2000);
	std::jthread jt([h=hd.native_handle()]() mutable
	{
		fast_io::obuf_file obf("client.txt");
		transmit(obf,h);
	});
	for(std::size_t i{};i!=10000000;++i)
		println(hd,i);
}