#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"
#include"../../include/fast_io_crypto.h"

int main()
try
{
	std::string key("abcdefgsdaiagojadsiogdasjiogadsjiogjadsiogjdasoigjadsogjasdiogjadsiogjiosdj");
	{
	fast_io::crypto::oone_time_pad<fast_io::obuf> ootp(key,u8"a.txt");
	print(ootp,u8"hello world\n");
	}
	{
	fast_io::crypto::ione_time_pad<fast_io::ibuf> iotp(key,u8"a.txt");
	transmit(fast_io::out,iotp);
	}
}
catch(std::exception const & e)
{
	println(fast_io::err,e);
	return 1;
}