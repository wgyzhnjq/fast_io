#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"

int main()
try
{
	fast_io::ibuf ib(u8"tb1.txt");
	fast_io::obuf ob(u8"gen1.txt");
	for(std::size_t i{};i!=128;++i)
	{
		std::string line;
		getline(ib,line);
		fast_io::istring_view isv(line);
		std::string a,b;
		scan(isv,a,b);
		if(b!="1A")
			println(ob,u8"case 0x",a,u8":return 0x",b,u8";");
	}
	println(ob,u8"default:return 0x1A;");
}
catch(std::exception const& e)
{
	println(fast_io::err,e);
	return 1;
}