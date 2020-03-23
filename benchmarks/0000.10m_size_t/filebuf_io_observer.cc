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
	fast_io::timer t("std::ofstream");
	std::ofstream fout("filebuf_io_observer.txt",std::ofstream::binary);
	fast_io::filebuf_io_observer fob(fout.rdbuf());
	for(std::size_t i(0);i!=N;++i)
		println(fob,i);
	}

}