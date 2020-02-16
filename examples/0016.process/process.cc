#include"../../include/fast_io.h"

int main()
try
{
	fast_io::posix_process process;
	if(is_child(process))
		fast_io::posix_exec("/usr/local/bin/gcc",{"-v"});
	wait(process);
}
catch(std::exception const& e)
{
	println(fast_io::err,e);
	return 1;
}