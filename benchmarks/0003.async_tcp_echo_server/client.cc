#include"../../include/fast_io_network.h"
#include"../../include/fast_io_device.h"
#include"../timer.h"

int main()
{
	fast_io::timer tm("fast_io tcp multithread");
	fast_io::ibuf_tcp_client hd(fast_io::ipv4{192,168,0,54},2000);
	print(hd,"Hello World\n");
	std::string str;
	scan(hd,str);
	println(str);
}