#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"

int main()
{
	fast_io::oascii_to_ebcdic<fast_io::obuf> ebcdic_obuf("ebcdic.txt",native_stdout_number);
	println(ebcdic_obuf,"weqtqewtqewt");
}