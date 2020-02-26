#include"../../include/fast_io_legacy.h"

int main()
{
	fast_io::c_file cfl("cfile.txt",fast_io::open_interface<fast_io::open_mode::out>);
	print(cfl,"Hello World\n");
}
