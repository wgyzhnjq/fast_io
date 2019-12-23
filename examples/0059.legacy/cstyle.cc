#include"../../include/fast_io_legacy.h"

int main()
{
	println(fast_io::c_stdout,"Hello World");		// call fast_io's println
	auto posix_handle(static_cast<fast_io::posix_io_handle>(fast_io::c_stdout));
	println(posix_handle,u8"dsgsdg\n");
}