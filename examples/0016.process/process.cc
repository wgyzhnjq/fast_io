#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"

int main()
try
{
	fast_io::native_pipe pipe;
	fast_io::jprocess process("/usr/local/bin/g++",{"--version"},{.out=pipe});
	fast_io::native_pipe pipe1;
	fast_io::jprocess process1("./transmit",{},{.in=pipe,.out=pipe1});
	fast_io::onative_file onv("on.txt");
	transmit(onv,pipe1);
}
catch(std::exception const& e)
{
	println(fast_io::err,e);
	return 1;
}