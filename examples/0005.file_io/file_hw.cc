#include"../../include/fast_io_device.h"

int main()
{
	fast_io::obuf ob("hw.txt");
	print(ob,"Hello World\n");
}