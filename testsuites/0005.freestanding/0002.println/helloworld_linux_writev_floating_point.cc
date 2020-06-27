#include"../../../include/fast_io.h"

extern "C" [[noreturn]] void _start() noexcept
{
	println(fast_io::out(),"Hello World\n",5,"\nLOL",4.1489124809);
	fast_io::fast_exit(0);
}

// g++ -o helloworld_linux_writev_floating_point helloworld_linux_writev_floating_point.cc -Ofast -std=c++2a -s -fno-exceptions -fno-rtti -DNDEBUG -nodefaultlibs -static -lgcc -nostartfiles -DFAST_IO_OPTIMIZE_SIZE

/*
$ ldd ./helloworld_linux_writev
        not a dynamic executable
*/
