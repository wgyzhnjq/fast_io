#include"../../include/fast_io_network.h"
#include"../../include/fast_io_device.h"
#include"../timer.h"

int main()
{
	fast_io::timer tm("fast_io tcp multithread");
	fast_io::obuf_file obf("client.txt");
	fast_io::tcp_client hd(fast_io::ipv4{127,0,0,1},2000);
	print(hd,"Hello World\n");
	transmit(obf,hd);
}