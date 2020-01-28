//windows only

#include"../../include/fast_io_legacy.h"

int main()
{
	fast_io::c_style_file csf(fast_io::posix_stdout(),fast_io::open::interface<fast_io::open::out|fast_io::open::binary>);
	print(csf,"Hello World from fast_io::print\n");
	fprintf(csf,"Hello World from std::fprintf\n");
}