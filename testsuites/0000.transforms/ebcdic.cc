#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"

int main()
{
	fast_io::iascii_to_ebcdic<fast_io::ibuf_file> ibf("ite.txt");
	fast_io::oebcdic_to_ascii<fast_io::obuf_file> obf("ote.txt");
	transmit(obf,ibf);
}
