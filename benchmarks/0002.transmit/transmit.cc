#include"../timer.h"
#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"
#include"../../include/fast_io_legacy.h"
#include"../../include/fast_io_crypto.h"
#include<fstream>

int main()
try
{
	{
		fast_io::timer tm("obuf_file <= ibuf_file");
		fast_io::ibuf_file ib("large_file.txt");
		fast_io::obuf_file ob("large_file_ibuf_to_obuf.txt");
		transmit(ob,ib);
	}
	{
		fast_io::timer tm("oascii_to_ebcdic<obuf_file> <= ibuf_file");
		fast_io::ibuf_file ib("large_file.txt");
		fast_io::oascii_to_ebcdic<fast_io::obuf_file> ob("large_file_ebcdic.txt");
		transmit(ob,ib);
	}
	{
		fast_io::timer tm("oascii_to_ebcdic<obuf_file> <= ascii_to_ebcdic<ibuf_file>");
		fast_io::iascii_to_ebcdic<fast_io::ibuf_file> ib("large_file.txt");
		fast_io::oascii_to_ebcdic<fast_io::obuf_file> ob("large_file_ascii_ebcdic.txt");
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
	{
		fast_io::timer tm("spec enc 128 128 cbc crypt obuf file <= ibuf_file");
		std::array<unsigned char, 16> key{1,2,3,4,5,6,7,8,1,2,3,4,5,6,7,8};
		std::array<unsigned char, 16> iv{1,2,3,4,5,6,7,8,1,2,3,4,5,6,7,8};
		fast_io::ibuf_file ib("large_file.txt");
		fast_io::crypto::ocbc_encrypt<fast_io::obuf_file, fast_io::crypto::speck::speck_enc_128_128> 
		ob(std::piecewise_construct,std::forward_as_tuple("cbc_encrypt.txt"),
		std::forward_as_tuple(std::as_writable_bytes(std::span(key)), std::as_writable_bytes(std::span(iv))));
		transmit(ob,ib);
	}
	{
		fast_io::timer tm("aes_enc_128 cbc crypt obuf file <= ibuf_file");
		std::array<unsigned char, 16> key{1,2,3,4,5,6,7,8,1,2,3,4,5,6,7,8};
		std::array<unsigned char, 16> iv{1,2,3,4,5,6,7,8,1,2,3,4,5,6,7,8};
		fast_io::ibuf_file ib("large_file.txt");
		fast_io::crypto::ocbc_encrypt<fast_io::obuf_file, fast_io::crypto::aes::aes_enc_128> 
		ob(std::piecewise_construct,std::forward_as_tuple("aes_encrypt.txt"),
		std::forward_as_tuple(std::as_writable_bytes(std::span(key)), std::as_writable_bytes(std::span(iv))));
		transmit(ob,ib);
	}
}
catch(std::exception const& e)
{
	println(fast_io::err,e);
}
