#include"../../include/fast_io.h"
#include"../../include/fast_io_legacy.h"

int main()
{
	transmit(fast_io::out,fast_io::c_stdin);
}
/*
Transmit from stdin to fast_io::out

Should be zero copy IO on linux
*/