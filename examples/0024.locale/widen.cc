#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"
#include<iostream>
#include"../../include/fast_io_legacy.h"

int main()
{
	fast_io::cpp_locale_wrapper<fast_io::native_io_handle> obf(fast_io::native_stdout_number);
	std::locale::global(std::locale(""));
	print(obf,4.46461245325);
	
}