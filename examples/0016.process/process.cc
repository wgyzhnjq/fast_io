#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"

int main()
try
{
	fast_io::native_pipe pipe;
	fast_io::jprocess process("/usr/local/bin/g++",
		{"--version"},{.out=pipe});
	transmit(fast_io::out,pipe);
	println("Process ID:",process.id());
}
catch(std::exception const& e)
{
	println(fast_io::err,e);
	return 1;
}