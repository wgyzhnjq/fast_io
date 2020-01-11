#include"../../include/fast_io.h"

//Compilation success
//g++ -o linux_freestanding_number_output linux_freestanding_number_output.cc -O3 -std=c++2a -fconcepts -nostartfiles -ffreestanding -fno-exceptions -fno-rtti
//Execuation:./linux_freestanding_number_output; echo $?

extern "C"
{
void _start() noexcept
{
	println(23451235);
	fast_io::fast_exit(0);
}
}