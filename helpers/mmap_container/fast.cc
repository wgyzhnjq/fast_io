#include"../timer.h"
#include<vector>
#include"page_alloc.h"

int main()
{
	constexpr std::size_t N{100000};
	constexpr std::size_t M{100};

	std::size_t sum{};
	fast_io::timer tm("std::vector");

	for(std::size_t i{};i!=N;++i)
	{
		std::size_t* bt{new std::size_t[M]};
		bt[M-1]={};
		sum += bt[M>>1];
		delete[] bt;
	}
}