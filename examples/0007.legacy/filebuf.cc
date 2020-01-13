#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"
#include"../../include/fast_io_legacy.h"


int main()
{
	fast_io::posix_file pf("fb.txt",fast_io::open::interface<fast_io::open::out|fast_io::open::binary>);
	fast_io::filebuf_handle hd(pf,std::ofstream::out|std::ofstream::binary);
}

