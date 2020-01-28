//windows only

#include"../../include/fast_io.h"

int main()
{
	fast_io::posix_file pf(fast_io::win32_stdout(),fast_io::open::interface<fast_io::open::out|fast_io::open::binary>);
	print(pf,"Hello World\n");
}