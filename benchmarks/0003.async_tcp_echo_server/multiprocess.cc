#include"../../include/fast_io_hosted.h"
#include"../../include/fast_io_device.h"
#include"../../include/fast_io.h"

int main()
try
{
	fast_io::onative_file log("log.txt");
	std::vector<fast_io::jprocess> vec;
	for(std::size_t i(2000);i!=3500;++i)
		vec.emplace_back(fast_io::jprocess("./server",{fast_io::concat(i)},{.err=log}));
}
catch(std::exception const& e)
{
	println(fast_io::err,e);
	return 1;
}