//Only usable in windows
#include"../../include/fast_io_box.h"

int main()
{
	print(fast_io::debug_box,"Hello World\n");
}

//This will fail to compile in Release (-DNDEBUG mode)