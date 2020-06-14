#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"
#include"../../include/fast_io_legacy.h"
#include <string>

int main()
{
	fast_io::ibuf_utf8_file<char32_t> utf8_file("utf_file.txt");
	for(std::u32string utf32str;scan<true>(utf8_file,fast_io::line(utf32str));println(fast_io::code_cvt(utf32str)));
}