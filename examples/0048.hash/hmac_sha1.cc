#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"
#include"../../include/fast_io_hash.h"

int main()
try
{
	fast_io::hmac_sha1 hmacs1("12345");
	print(hmacs1,"abc");
	flush(hmacs1);
	println(fast_io::out,hmacs1);
}
catch(std::exception const& e)
{
	println(fast_io::err,e);
	return 1;
}