#include"../../include/fast_io.h"
#include"../../include/fast_io_core_impl/ebcdic.h"
#include"../../include/fast_io_device.h"

int main()
{
	fast_io::ebcdic<fast_io::obuf> eh(u8"abc.txt");
	put(eh,0x99);
}