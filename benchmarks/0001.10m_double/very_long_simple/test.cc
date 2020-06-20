#include"../../../include/fast_io.h"
#include"../../../include/fast_io_device.h"

int main()
{
	fast_io::ibuf_file ibf("testsuite.in");
	double d;
	scan(ibf,d);
	println(d," ",122266362814542312101747538660479311766991132798333077273892284899461673161145784942495.0);
}