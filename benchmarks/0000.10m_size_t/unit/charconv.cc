#include<charconv>
#include"../../include/fast_io_device.h"

int main()
{
	constexpr std::size_t n{10000000};
	fast_io::obuf_file obf("charconv.txt");
	std::array<char,50> buffer;
	for(std::size_t i{};i!=n;++i)
	{
		auto p=std::to_chars(buffer.data(),buffer.data()+buffer.size(),i);
		*p=u8'\n';
		write(obf,buffer.data(),++p);
	}
}