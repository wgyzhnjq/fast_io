#include"../../../include/fast_io.h"
#include"../../../include/fast_io_device.h"

int main()
{
	fast_io::ibuf_file ibf("testsuite.in");
	double d;
	scan(ibf,d);
	println(d," ",04190920254308689.2080128068326384397330500);
}