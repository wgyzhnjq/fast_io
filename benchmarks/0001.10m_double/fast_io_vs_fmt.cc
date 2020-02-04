#include"../timer.h"
#include<fmt/format.h>
#include<random>

int main()
{
	constexpr std::size_t N(10000000);
	std::vector<double> vec;
	vec.reserve(N);
	std::random_device device;
	std::mt19937_64 eng(device());
	std::uniform_real_distribution dis(-1000.0,1000.0);
	for(std::size_t i(0);i!=N;++i)
		vec.emplace_back(dis(eng));
	std::size_t fast_io_value{};
	{
	fast_io::timer t("fast_io::to");
	for(std::size_t i{};i!=N;++i)
		fast_io_value+=fast_io::concat(vec[i]).size();
	}
	std::size_t fmt_value{};
	{
	fast_io::timer t("fmt::format");
	for(std::size_t i{};i!=N;++i)
		fmt_value+=fmt::format("{}",vec[i]).size();
	}

	println("fast_io::to value:",fast_io_value,"\nfmt::format value:",fmt_value);

}