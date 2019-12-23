#include"../../include/fast_io.h"

int main()
try
{
	fast_io::win32_file file(u8"abc.txt",u8"wx");
	print(file,u8"hello world\n");
}
catch(std::exception const & e)
{
	println(fast_io::out,e);
	return 1;
}