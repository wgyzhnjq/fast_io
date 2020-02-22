#include"../../include/fast_io.h"

extern "C" void _start() noexcept
{
	print("Hello World\n");
	fast_io::fast_terminate(0);
}

//g++ -o helloworld_linux helloworld_linux.cc -Ofast -std=c++2a -s -flto -fno-exceptions -fno-rtti -DNDEBUG