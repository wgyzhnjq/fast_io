
#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"
#include"../../include/fast_io_legacy.h"
#include"../timer.h"
int main()
{
	fast_io::timer tm("c_file");
	fast_io::c_file cf("c_file.txt",fast_io::open_interface<fast_io::open_mode::out|fast_io::open_mode::binary>);
	for(std::size_t i{};i!=10000000;++i)
		print(cf,fast_io::width(i,20,false,u8'0'));
}