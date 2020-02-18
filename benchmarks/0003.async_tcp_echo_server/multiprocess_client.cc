#include"../../include/fast_io_hosted.h"
#include"../../include/fast_io_device.h"

int main()
{
	fast_io::onative_file log("client_log.txt");
	std::vector<fast_io::jprocess> vec;
	for(std::size_t i(2000);i!=3500;++i)
	{
		fast_io::jprocess proc("./client",{fast_io::concat(i)},{.err=log});
		vec.emplace_back(std::move(proc));
	}
}