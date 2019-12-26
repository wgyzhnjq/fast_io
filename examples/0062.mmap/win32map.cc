#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"

int main()
try
{
	fast_io::system_file sys_file("abc.txt","w+");
	fast_io::win32_file_map win32_map(sys_file,fast_io::file_map_attribute::read_write,8192);
	win32_map=fast_io::win32_file_map(sys_file,fast_io::file_map_attribute::read_write,16384);
	win32_map=fast_io::win32_file_map(sys_file,fast_io::file_map_attribute::read_write,32768);
	auto mem(win32_map.region());
	for(auto& e : mem)
		e=std::byte{'a'};
}
catch(std::exception const& e)
{
	println(fast_io::err,e);
	return 1;
}