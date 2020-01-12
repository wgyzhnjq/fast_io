#include"../../include/fast_io.h"
#include"../../include/fast_io_in.h"

int main()
{
	double d;
	scan(d);
	println(fast_io::scientific<6>(d));
}