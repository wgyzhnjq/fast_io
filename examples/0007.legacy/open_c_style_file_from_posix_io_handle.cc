#include"../../include/fast_io_legacy.h"

int main()
{
	fast_io::posix_io_handle posix_out(fast_io::posix_out_number);	//stdout number 1
	fast_io::c_style_file csf(std::move(posix_out),fast_io::open::out|fast_io::open::binary);	//call fdopen. We can use C++ style open mode
	print(csf,"Hello World from fast_io::print\n");
	fprintf(csf,"Hello World from std::fprintf\n");
}