#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"
#include"../../include/fast_io_legacy.h"

int main()
{
//assume c_style_file is encoded with ebcdic even it is not.
	fast_io::ebcdic<fast_io::c_style_file> eh(u8"ebcdic_coded_file.txt",u8"rb");
	std::u8string str;
	scan(eh,str);
	println(fast_io::out,str);
}