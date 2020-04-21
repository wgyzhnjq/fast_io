#include"../timer.h"
#include<vector>
#include"page_alloc.h"

struct sf
{
	std::size_t v;
	constexpr sf();
};

int main()
{
	constexpr std::size_t N{1};
	constexpr std::size_t M{1000000};

	std::size_t sum{};
	fast_io::timer tm("std::vector");
	std::vector<std::size_t> vec(M);

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