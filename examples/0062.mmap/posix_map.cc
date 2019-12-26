#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"

int main()
try
{
	fast_io::native_file sys_file("abc.txt","rw");
	fast_io::posix_file_map posix_map(sys_file,fast_io::file_map_attribute::read_write,8192);
	auto mem(posix_map.region());
	for(auto& e : mem)
		e=std::byte{'a'};
}
catch(std::exception const& e)
{
	println(fast_io::err,e);
	return 1;
}