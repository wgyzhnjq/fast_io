#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"
#include"../../include/fast_io_legacy.h"

int main()
{
	fast_io::obuf_file obf("fast_io_stream.txt");
	iostream_ref(obf)<<"Hello World\n";
}
