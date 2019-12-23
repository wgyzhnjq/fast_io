
#include"../../include/fast_io.h"

#include <array>
#include <string>

int main()
try
{
	std::array<uint8_t, 32> key = {'8',u8'3',u8'3',u8'4',u8';',u8'2',u8'3',u8'4',u8'a',u8'2',u8'c',u8'4',u8']',u8'0',u8'3',u8'4',u8'8',u8'3',u8'3',u8'4',u8';',u8'2',u8'3',u8'4',u8'a',u8'2',u8'c',u8'4',u8']',u8'0',u8'3',u8'4'};
	{
		fast_io::crypto::basic_oecb<fast_io::obuf, fast_io::crypto::speck::speck_enc_128_256> enc_stream(key, "ecb.out");
		put(enc_stream,u8'a');
		print(enc_stream, "bc");
		print(enc_stream, "defghijklmnop12345678123456");
		print(enc_stream, "78ponmlkjihgfedcba");
		print_flush(enc_stream, "xx\n");
	}
	
	{
		fast_io::crypto::basic_iecb<fast_io::ibuf, fast_io::crypto::speck::speck_dec_128_256> dec_stream(key, "ecb.out");
		transmit(fast_io::out, dec_stream);
	}
}
catch(std::exception const & e)
{
	println(fast_io::err,e);
	return 1;
}
