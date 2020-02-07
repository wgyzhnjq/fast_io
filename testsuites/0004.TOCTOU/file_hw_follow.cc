#include"../../include/fast_io_device.h"

int main()
{
	fast_io::obuf_file obf("hw.txt",fast_io::open::interface<fast_io::open::out|fast_io::open::trunc|fast_io::open::reparse_point>);
	print(obf,"Hello World\n");
}