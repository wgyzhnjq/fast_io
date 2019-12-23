#include"../../include/fast_io.h"
#include<filesystem>

int main()
try
{
	fast_io::obuf process_log("process_log.txt");
	for(auto const & e : std::filesystem::directory_iterator("/proc/"))
	{
		auto stem(e.path().stem());
		auto string(stem.string());
		for(auto const & e : string)
			if(10<=static_cast<unsigned char>(e)-'0')
				goto nextloop;
		{
		println(process_log,u8"Process ID: ",string);
		{
		println(process_log,u8"\nStatus");
		fast_io::isystem_file ib(fast_io::concat<>("/proc/",string,u8"/status"));
		transmit(process_log,ib);
		println(process_log);
		}
		{
		println(process_log,u8"Maps");
		fast_io::isystem_file ib(fast_io::concat<>("/proc/",string,u8"/maps"));
		transmit(process_log,ib);
		}
		println(process_log,u8"\n");
		}
		nextloop:;
	}
}
catch(std::exception const & e)
{
	println(fast_io::err,e);
	return 1;
}
