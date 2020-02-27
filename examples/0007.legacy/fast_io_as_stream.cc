#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"
#include"../../include/fast_io_legacy.h"

int main()
{
	fast_io::fast_io_streambuf<fast_io::obuf_file> strmbf("fast_io_stream.txt");
	std::ostream osm(std::addressof(strmbf));
	osm<<"Hello World\n";
}