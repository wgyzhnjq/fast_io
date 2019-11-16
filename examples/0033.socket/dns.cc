
#include"../../include/fast_io.h"
#include"../../include/fast_io_network.h"

int main()
try
{
	fast_io::ipv4_dns d("www.baidu.com");
    for(auto const& addr:d)
    {
        println(fast_io::out, addr);
    }
}
catch(std::system_error const & e)
{
	println(fast_io::err,e);
	return 1;
}
