#include<iostream>
#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"

int main()
try
{
	fast_io::omap sys_file("w.txt","w+");
	for(std::size_t i(0);i!=10000;++i)
	print(sys_file,u8"Hello World");
}
catch(std::exception const& e)
{
	println(fast_io::err,e);
}