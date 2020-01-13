#include"../../include/fast_io_legacy.h"

int main()
{
	fprintf(fast_io::c_stdout,"%d\n",6);		//call standard function but checking error
	println(fast_io::c_stdout,u8"Hello World");		// call fast_io's println
	fast_io::posix_io_handle posix_handle(fast_io::c_stdout);	//gets its internal posix handle
	println(posix_handle,u8"dsgsdg");
	transmit(fast_io::c_stdout,fast_io::c_stdin);
}