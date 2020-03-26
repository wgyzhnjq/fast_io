#include"../../include/fast_io.h"
#include"../../include/fast_io_legacy.h"

int main()
try
{
	fast_io::c_file cf(fast_io::c_file_cookie,std::in_place_type<fast_io::posix_file>,
		"c_file_cookie_posix_file.txt",fast_io::open_interface<fast_io::open_mode::out|fast_io::open_mode::binary>);
	println(cf,"Hello World");
}
catch(std::system_error const& e)
{
	println(fast_io::err,e);
}