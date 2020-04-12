
#include"../timer.h"
#include"mmap_svector.h"
#include<vector>

int main()
{
	constexpr std::size_t N{10485760};
	{
		fast_io::timer tm("mmap_svector");
		fast_io::mmap_svector<std::size_t,256> vec;
		for(std::size_t i{};i!=N;++i)
			vec.emplace_back(235);
	}
	{
		fast_io::timer tm("std::vector");
		std::vector<std::size_t> vec;
		for(std::size_t i{};i!=N;++i)
			vec.emplace_back(235);
	}

//	println(vec.size()," ",vec.capacity());
//	for(auto const& e : vec)
//		println(e);
}