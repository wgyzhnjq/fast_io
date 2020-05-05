#include"../../include/fast_io_device.h"
#include"../../include/fast_io.h"
#include"../../include/fast_io_driver/win32_memory.h"

int main()
try
{
	fast_io::win32_memory_file wmf(fast_io::win32_desired_access::process_vm_read,false,17536,0x22ff0c);
	fast_io::win32_virtual_protect_guard guard(wmf,4096,fast_io::win32_memory_page_protect::readwrite);
	auto info{fast_io::win32_virtual_query(wmf)};
	print(info);
	fast_io::onative_file onv("mem_content.txt");
	transmit(onv,wmf);
}
catch(std::exception const& e)
{
	println(fast_io::err,e);
	return 1;
}