#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"

int main()
{
	fast_io::oebcdic_to_ascii<fast_io::log_type> ebcdic(fast_io::native_stdout_number);
	print(ebcdic,"\xC8\x85\x93\x93\x96\x40\xC5\xC2\xC3\xC4\xC9\xC3\x15");
/*
Should print u8"Hello EBCDIC\r\n"
Notice that 0x15 is NL in EBCDIC which should output \r\n in ASCII;
*/
}