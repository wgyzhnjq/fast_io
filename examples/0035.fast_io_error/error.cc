#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"


int main()
try
{
	fast_io::ibuf_file ibf("w.txt");
}
catch(fast_io::fast_io_error const& e)
{
	println_err(e);
	return 1;
}