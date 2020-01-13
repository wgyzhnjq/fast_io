#include"../timer.h"
#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"
#include"../../include/fast_io_legacy.h"
#include<fstream>

int main()
try
{
	{
		std::string str;
		fast_io::timer tm("ostring <= ibuf_file");
		fast_io::ibuf_file ib("large_file.txt");
		scan(ib,fast_io::whole(str));
	}
	{
		std::string str;
		fast_io::timer tm("ostring <= ibuf_file");
		fast_io::ibuf_file ib("large_file.txt");
		fast_io::ostring ostr;
		transmit(ostr,ib);
		str=std::move(ostr.str());
	}
	{
		std::string str;
		fast_io::timer tm("ostring <= inative_file");
		fast_io::inative_file ib("large_file.txt");
		fast_io::ostring ostr;
		transmit(ostr,ib);
		str=std::move(ostr.str());
	}
	{
		fast_io::timer tm("ofstream <= ifstream");
		std::ifstream ifst("large_file.txt",std::ifstream::binary);
		std::ofstream ofst("large_file_stream.txt",std::ofstream::binary);
		ofst<<ifst.rdbuf();
	}
	{
		fast_io::timer tm("obuf_file <= ibuf_file");
		fast_io::ibuf_file ib("large_file.txt");
		fast_io::obuf_file ob("large_file_ibuf_to_obuf.txt");
		transmit(ob,ib);
	}
	{
		fast_io::timer tm("oascii_to_ebcdic<obuf_file> <= ibuf_file");
		fast_io::ibuf_file ib("large_file.txt");
		fast_io::oascii_to_ebcdic<fast_io::obuf_file> ob("large_file_ibuf_to_obuf.txt");
		transmit(ob,ib);
	}
	{
		fast_io::timer tm("onative_file <= inative_file");
		fast_io::inative_file ib("large_file.txt");
		fast_io::onative_file ob("large_file_inative_file_to_onative_file.txt");
		transmit(ob,ib);
	}
	{
		fast_io::timer tm("c_style_file <= ibuf_file");
		fast_io::ibuf_file ib("large_file.txt");
		fast_io::c_style_file csf("large_file_ibuf_to_c_style_file.txt","wb");
		transmit(csf,ib);
	}
}
catch(std::exception const& e)
{
	println(fast_io::err,e);
}
