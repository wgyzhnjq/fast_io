#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"
#include"../../include/fast_io_legacy.h"
#include<thread>

int main()
try
{
//	fast_io::pipe pipe;
//	print("create process before");
//	fast_io::onative_file onf("b.txt");
	fast_io::win32_process process("g++ --version",
			{.out=fast_io::out,
			.err=fast_io::err});
//	pipe.out().close();
//	transmit(fast_io::out,pipe);
	process.join();
}
catch(std::exception const& e)
{
	println(fast_io::err,e);
	return 1;
}