#include"../../include/fast_io_in.h"
#include"../../include/fast_io_legacy.h"

int main()
{
	transmit(fast_io::c_stdout,fast_io::in);
}
/*
Transmit from fast_io::in to C stdout

Should be zero copy IO on linux
*/