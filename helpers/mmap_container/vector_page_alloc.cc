#include"../timer.h"
#include<vector>
#include"page_alloc.h"

int main()
{
	constexpr std::size_t N{1000};
	constexpr std::size_t M{1000000};

	std::size_t sum{};

	std::vector<std::size_t> vec(M);
	fast_io::timer tm("std::vector");
	for(std::size_t i{};i!=N;++i)
	{
	for(auto & e : vec)
	{
		sum+=e;
		e=sum;
	}
	}
	println("sum:",sum);
}