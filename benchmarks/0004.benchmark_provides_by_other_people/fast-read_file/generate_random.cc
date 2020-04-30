#include"../../timer.h"
#include"../../../include/fast_io.h"
#include"../../../include/fast_io_device.h"
#include"../../../include/fast_io_legacy.h"
#include<random>

int main()
{
	constexpr std::size_t N(10000000);
	std::mt19937_64 eng;
	std::uniform_int_distribution dis(std::numeric_limits<int>::min(),
		std::numeric_limits<int>::max());
	{
	fast_io::timer t("output");
	fast_io::obuf_file obf("data.txt");
	fast_io::c_file dc("datac.txt","wb");
	for(std::size_t i{};i!=N;++i)
	{
		auto e{dis(eng)};
		println(obf,e);
		fprintf(dc.native_handle(),"%d\n",e);
	}
	}
}