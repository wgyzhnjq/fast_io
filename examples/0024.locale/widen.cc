#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"
#include<iostream>
#include"../../include/fast_io_legacy.h"

int main()
{
	fast_io::cpp_locale_wrapper<fast_io::obuf_file> obf("a.txt");
	print(obf,4);
}