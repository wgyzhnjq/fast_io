#include<vector>
#include"timer.h"

int main()
{
	constexpr std::size_t N(100000000);
{
	cqw::timer tm("vector");
	std::vector<std::size_t> vec;
	vec.reserve(N);
	for(std::size_t i(0);i<N;i+=4)
	{
		vec.emplace_back(i);
		vec.emplace_back(i+1);
		vec.emplace_back(i+2);
		vec.emplace_back(i+3);
	}

}
{
	cqw::timer tm("vector emplace_back_unchecked");
	std::vector<std::size_t> vec;
	vec.reserve(N);
	for(std::size_t i(0);i<N;i+=4)
	{
		vec.emplace_back_unchecked(i);
		vec.emplace_back_unchecked(i+1);
		vec.emplace_back_unchecked(i+2);
		vec.emplace_back_unchecked(i+3);
	}
}


}