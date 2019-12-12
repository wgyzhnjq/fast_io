#include<vector>
#include"timer.h"

int main()
{
	constexpr std::size_t N(100000000);
{
	cqw::timer tm("vector");
	std::vector<std::size_t> vec;
	vec.reserve(N);
	for(std::size_t i(0);i!=N;++i)
		vec.emplace_back(i);

}
{
	cqw::timer tm("vector emplace_back_unchecked");
	std::vector<std::size_t> vec;
	vec.reserve(N);
	for(std::size_t i(0);i!=N;++i)
		vec.emplace_back_unchecked(i);
}


}