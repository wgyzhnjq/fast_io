#include"../../include/fast_io.h"
#include"../../include/fast_io_async.h"
#include"../../include/fast_io_device.h"
#include"../../include/fast_io_network.h"

int main()
try
{
	fast_io::server server(2000,fast_io::sock::type::stream);
	println(fast_io::out,"ultimate: ",ultimate_native_handle(server));
	fast_io::epoll::handle_pool pool(500);
	add_control(pool,server,fast_io::epoll::event::in,0);

	using namespace std::chrono_literals;
/*	for(;;)
	{
	fast_io::acceptor_buf acc(server,fast_io::non_block);

		auto nfds(wait(pool,fast_io::epoll::event::in,5,1,5s));
		for(std::size_t i(0);i!=nfds;++i)
		{

		}
	}*/
}
catch(std::exception const & e)
{
	println(fast_io::err,e);
	return 1;
}