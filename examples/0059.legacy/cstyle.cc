#include"../../include/fast_io_legacy.h"

int main()
{
	fprintf(fast_io::c_stdout,"%d\n",6);		//call standard function but checking error
	println(fast_io::c_stdout,"Hello World");		// call fast_io's println
//cast c_style_io_handle to posix handle for c_stdout. Negative effect for coding in a EBIDIC system. DON'T DO THIS IF YOU DON'T NEED IT
	fast_io::posix_io_handle posix_handle(fast_io::c_stdout);
	println(posix_handle,u8"dsgsdg\n");//posix_handle is utf-8 coding
}