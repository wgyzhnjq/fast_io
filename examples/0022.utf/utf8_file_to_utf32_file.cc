#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"

int main(int argc,char** argv)
try
{
	if(argc!=3)
	{
		print(fast_io::err,"Usage: ",*argv," <source UTF8 file name> <dest UTF32 (",std::endian::native==std::endian::little?"LE":"BE",") file name>\n");
		return 1;
	}
	fast_io::ibuf_utf8_file<char32_t> utf8_file_in(argv[1]);
	fast_io::basic_obuf_file<char32_t> u32_file(argv[2]);
	char32_t const bom{65279};	//write bom. 65279 is the code page for UTF-16 and UTF-32
	write(u32_file,std::addressof(bom),std::addressof(bom)+1);
	transmit(u32_file,utf8_file_in);
}
catch(std::exception const& e)
{
	println(fast_io::err,e);
	return 2;
}