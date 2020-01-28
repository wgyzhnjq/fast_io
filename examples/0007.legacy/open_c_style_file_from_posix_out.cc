#include"../../include/fast_io_legacy.h"

int main()
{
	fast_io::posix_io_handle posix_out(1);	//stdout number
	fast_io::c_style_file csf(std::move(posix_out),"wb");
	print(csf,"Hello World from fast_io::print\n");
	fprintf(csf,"Hello World from std::fprintf\n");
}