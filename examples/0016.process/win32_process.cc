#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"

int main()
try
{
	fast_io::pipe pipe;
	fast_io::win32_process process("g++ --version",
			{.out=pipe.out(),
			.err=fast_io::err});
	pipe.in().close();
	process.join();
	transmit(fast_io::out,pipe);
}
catch(std::exception const& e)
{
	println(fast_io::err,e);
	return 1;
}