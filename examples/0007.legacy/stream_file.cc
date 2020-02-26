#include"../../include/fast_io_legacy.h"

int main()
{
	fast_io::stream_file fsf("stream_filehw.txt",fast_io::open_interface<fast_io::open_mode::out>);
	print(fsf,"Hello, I am fast_io\n");
	fsf.native_handle()<<"Hello, I am stream\n";
	print(fsf,"Hello, I am fast_io2\n");
}
