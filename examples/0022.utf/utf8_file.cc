#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"
#include"../../include/fast_io_legacy.h"
#include <string>

int main()
{
	fast_io::ibuf_utf8_file<char32_t> ifile("utf_file.txt");
	for (std::u32string u32_str;scan<true>(ifile, fast_io::line(u32_str));)
	{
		println(fast_io::code_cvt(u32_str));
	}
}