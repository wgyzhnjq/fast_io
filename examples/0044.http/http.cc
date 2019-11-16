#include"../../include/fast_io_device.h"
#include"../../include/fast_io_network.h"
#include"../../include/fast_io.h"

int main()
try
{
	fast_io::client_buf hd(fast_io::dns_once("www.jszg.edu.cn"),80,fast_io::sock::type::stream);
	print_flush(hd,"GET /portal/home/index HTTP/1.1\r\n"
		"Host:www.jszg.edu.cn\r\n"
		"User-agent:whatever\r\n"
		"Accept-Type:*/*\r\n"
		"Connection:close\r\n\r\n");
	fast_io::obuf file("http.txt");
	transmit(file,hd);
}
catch(std::exception const& e)
{
	println(fast_io::err,e);
	return 1;
}