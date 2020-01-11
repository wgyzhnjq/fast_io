#include"../../include/fast_io.h"

//Compilation success
//g++ -o linux_freestanding_number_output linux_freestanding_number_output.cc  -Ofast -std=c++2a -s -fno-exceptions -fno-rtti -ffreestanding -nostartfile
//Execuation:./linux_freestanding_number_output
//ryu algorithm

extern "C"
{
void _start() noexcept
{
	println(23451235.12421);
	fast_io::fast_exit(0);
}
}