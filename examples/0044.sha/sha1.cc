#include"../../include/fast_io.h"
#include"../../include/fast_io_crypto/hash/sha.h"

int main()
{
	fast_io::sha1 sha;
	{
	fast_io::block_processor processor(sha);
	for(std::size_t i{};i!=100;++i)
	print(processor,"Hello World");
	}
	debug_print("before crash??");
	println(sha);
}