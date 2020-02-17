#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"
#include"../../include/fast_io_legacy.h"
#include<thread>

int main()
try
{
	fast_io::native_pipe pipe;
	fast_io::win32_jprocess process("g++ --version",
			{.out=pipe.out(),.err=fast_io::err});
	pipe.out().close();
	transmit(fast_io::out,pipe);
	println("Process ID:",process.id());
}
catch(std::exception const& e)
{
	println(fast_io::err,e);
	return 1;
}