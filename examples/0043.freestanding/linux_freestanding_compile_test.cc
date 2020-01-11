#include"../../include/fast_io.h"

//g++ -o linux_freestanding_compile_test linux_freestanding_compile_test.cc -Ofast -std=c++2a -s -fno-exceptions -fno-rtti -ffreestanding -nostartfiles -static
//Compilation success

extern "C"
{
void _start() noexcept
{
	print("Hello World\n");
	fast_io::fast_exit(0);
}
}