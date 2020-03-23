#include"../timer.h"
#include<fstream>
#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"
//#include"../../include/fast_io_crypto.h"
#include"../../include/fast_io_legacy.h"

int main()
{
	std::size_t constexpr N(10000000);
	{
	fast_io::timer t("output");
	fast_io::c_file_unlocked cful_out("cful.txt",fast_io::open_interface<fast_io::open_mode::out|fast_io::open_mode::binary>);
	for(std::size_t i(0);i!=N;++i)
		println(cful_out,i);
	}

	std::vector<std::size_t> v(N);
	{
	fast_io::timer t("input");
	fast_io::c_file_unlocked cful("cful.txt",fast_io::open_interface<fast_io::open_mode::in|fast_io::open_mode::binary>);
	fast_io::c_file_unlocked cful_out("cful_verify.txt",fast_io::open_interface<fast_io::open_mode::out|fast_io::open_mode::binary>);
	for(auto const & e : igenerator(cful))
	{
		put(cful_out,e);
	}
	}
}