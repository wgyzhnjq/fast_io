#include"timer.h"
#include<fstream>
#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"
#include"../../include/fast_io_crypto.h"
#include"../../include/fast_io_legacy.h"
#include<exception>
#include<cmath>
#include<memory>
#include<cstdio>
#include<charconv>

int main()

{
	std::size_t constexpr N(10000000);
	{
	cqw::timer t(u8"obuf");
	fast_io::obuf obuf("obuf.txt");
	for(std::size_t i(0);i!=N;++i)
		println(obuf,i);
	}
	{
	cqw::timer t(u8"om");
	fast_io::omap om("omap.txt");
	reserve(om,N*20);
	for(std::size_t i(0);i!=N;++i)
		println(om,i);
	}
}