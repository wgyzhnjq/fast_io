#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"

int main()
try
{
	fast_io::native_pipe pipe;
	fast_io::posix_waiting_process process;
	if(is_child(process))
	{
		pipe.in().close();
		fast_io::posix_exec("/usr/local/bin/gcc",{"-v"});
	}
	pipe.out().close();
	fast_io::onative_file onv("pipe.txt");
	transmit(onv,pipe);
}
catch(std::exception const& e)
{
	println(fast_io::err,e);
	return 1;
}