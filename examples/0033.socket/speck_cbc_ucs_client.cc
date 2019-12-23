#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"
#include"../../include/fast_io_crypto.h"
#include"../../include/fast_io_network.h"

int main()
try
{
	//use speck cbc mode to encrypt our communication
	std::array<uint8_t, 16> constexpr key {'8',u8'3',u8'3',u8'4',u8';',u8'2',u8'3',u8'4',u8'a',u8'2',u8'c',u8'4',u8']',u8'0',u8'3',u8'4'};
	std::array<uint8_t, 16> constexpr iv {'1',u8'2',u8'3',u8'4',u8'1',u8'2',u8'3',u8'4',u8'1',u8'2',u8'3',u8'4',u8'1',u8'2',u8'3',u8'4'};
	fast_io::crypto::basic_icbc<fast_io::client_buf, fast_io::crypto::speck::speck_dec_128_128> hd(key,iv,fast_io::to<fast_io::ipv4>("127.0.0.1"),2000,fast_io::sock::type::stream);
	std::string str;
	fast_io::osystem_file ob("speck_decrypted.txt");
	transmit(ob,hd);
}
catch(std::exception const & e)
{
	println(fast_io::err,e);
	return 1;
}