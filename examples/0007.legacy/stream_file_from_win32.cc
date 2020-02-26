#include"../../include/fast_io_legacy.h"

int main()
{
	fast_io::win32_file winfile("stream_from_win32.txt",fast_io::open_interface<fast_io::open_mode::out>);
	print(winfile,"Hello, I am win32_file from fast_io\n");
	fast_io::stream_file fsf(std::move(winfile),fast_io::open_interface<fast_io::open_mode::out>);
	print(fsf,"Hello, I am fast_io\n");
	fsf.native_handle()<<"Hello, I am stream\n";
	print(fsf,"Hello, I am fast_io2\n");
}
