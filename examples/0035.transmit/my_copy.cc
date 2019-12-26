#include"../../include/fast_io.h"
#include<exception>
#include<chrono>
#include<string_view>

int main(int argc,char **argv)
try
{
	if(argc!=3)
	{
		fprint(fast_io::err,u8"Usage: % <source> <dest>\n",*argv);
		return 1;
	}
	std::string_view source_filename(argv[1]),dest_filename(argv[2]);
	auto t0(std::chrono::high_resolution_clock::now());
	std::size_t transmitted_bytes(0);
	{
		fast_io::native_file source(source_filename,fast_io::open::interface<fast_io::open::c_style("rb")>);
		fast_io::native_file dest(dest_filename,fast_io::open::interface<fast_io::open::c_style("wbx")>);
		transmitted_bytes=transmit(dest,source);
	}
	fprint(fast_io::out,u8"Copy % --> % Done (% bytes)\nTime elasped: %s\n",
				source_filename,dest_filename,transmitted_bytes,
				std::chrono::high_resolution_clock::now()-t0);
}
catch(std::exception const & e)
{
	println(fast_io::err,e);
	return 1;
}
