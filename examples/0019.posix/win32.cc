#include"../../include/fast_io.h"

int main()
{
	fast_io::posix_file pf("a.txt",fast_io::open_interface<fast_io::open_mode::out>);
	print(pf,"Hello World\n");
}