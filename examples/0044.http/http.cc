#include"../../include/fast_io_device.h"
#include"../../include/fast_io_network.h"
#include"../../include/fast_io.h"

int main()
try
{
	for(auto const& address:fast_io::dns("www.jszg.edu.cn"))
	{
		fast_io::client_buf hd(address,80,fast_io::sock::type::stream);
		print_flush(hd,"abc(GET / HTTP/1.1\r\n"
			"Host:www.jszg.edu.cn\r\n"
			"User-agent:whatever\r\n"
			"Accept-Type:*/*\r\n"
			"Connection:close\r\n\r\n");
		fast_io::obuf file("http.txt");
		transmit(file,hd);
	}
}
catch(std::exception const& e)
{
	println(fast_io::err,e);
	return 1;
}