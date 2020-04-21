#include"../timer.h"
#include<vector>

int main()
{
	constexpr std::size_t N{100};
	constexpr std::size_t M{10};

	std::size_t sum{};
	fast_io::timer tm("std::vector");

	for(std::size_t i{};i!=N;++i)
	{
		std::vector<std::size_t> vec(M);
		for(std::size_t j{};j!=M;++j)
		{
			sum+=vec.emplace_back(j);
		}
	}
	print(sum);
}