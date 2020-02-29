#include"../../include/fast_io_device.h"

int main()
{
	fast_io::omap_file pf("a.txt",fast_io::open_interface<fast_io::open_mode::trunc|fast_io::open_mode::in|fast_io::open_mode::out|fast_io::open_mode::binary>);
	print(pf,"Hello World\n");
}