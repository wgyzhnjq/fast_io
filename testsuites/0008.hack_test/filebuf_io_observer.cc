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
	std::ofstream fout("filebuf_io_observer.txt",std::ofstream::binary);
	fast_io::filebuf_io_observer fob(fout.rdbuf());
	for(std::size_t i(0);i!=N;++i)
		println(fob,i);
	}

	std::vector<std::size_t> v(N);
	{
	fast_io::timer t("input");
	std::ifstream fin("filebuf_io_observer.txt",std::ifstream::binary);
	fast_io::filebuf_io_observer fob(fin.rdbuf());
	fast_io::c_file_unlocked cful("cful.txt",fast_io::open_interface<fast_io::open_mode::out|fast_io::open_mode::binary>);
	for(auto const & e : igenerator(fob))
	{
//		scan(fob,v[i]);
//		println(cful,v[i]);
		put(cful,e);
	}
	}
}