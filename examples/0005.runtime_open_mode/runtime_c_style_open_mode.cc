#include"../../include/fast_io.h"

int main()
{
	fast_io::obuf ob("compile_time_open_mode.txt",u8"xb");
	//no need write wxb since w has already contained in the type information
}
