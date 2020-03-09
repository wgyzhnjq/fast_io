#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"
#include"../../include/fast_io_network.h"


int main()
try
{
	fast_io::ibuf_tcp_client hd(fast_io::dns_once("www.jgjy.gov.cn"),80);
	print(hd,u8"GET / HTTP/1.1\r\n"
		"Host:www.jgjy.gov.cn\r\n"
		"User-agent:whatever\r\n"
		"Accept-Type:*/*\r\n"
		"Connection:close\r\n\r\n");
	fast_io::http_status status;
	scan(hd,status);
	println("http version:",status.version,u8"\tcode is ",status.code);
	for(auto line:scan_http_header(hd))
		println(u8"key:",line.key,u8"\tvalue:",line.value);
}
catch(std::exception const& e)
{
	println(fast_io::err,e);
	return 1;
}