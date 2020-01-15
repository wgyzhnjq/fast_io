#include"../../include/fast_io_device.h"

int main()
{
	fast_io::obuf_file ob("large_file.txt");
	for(std::size_t i(0);i!=1000000ULL;++i)
		print(ob,"abcdefghijklmnopqrstuvwxyz\n");
}