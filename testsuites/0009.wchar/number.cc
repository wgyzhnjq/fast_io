#include<cstdio>
#include"../../include/fast_io_device.h"
#include"../../include/fast_io.h"
#include"../../include/fast_io_legacy.h"

int main()
{
	fast_io::wc_file baof("w.txt","wb");
	for(std::size_t i{};i!=10000000;++i)
		println(baof,i);
}