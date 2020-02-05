#include"../timer.h"
#include<fmt/format.h>
#include<random>

int main()
{
	constexpr std::size_t N(10000000);
	std::vector<std::size_t> vec;
	vec.reserve(N);
	std::random_device device;
	std::mt19937_64 eng(device());
	std::uniform_int_distribution<std::size_t> dis(0,std::numeric_limits<std::size_t>::max());
	for(std::size_t i(0);i!=N;++i)
		vec.emplace_back(dis(eng));
	std::size_t fast_io_ostr_value{};
	{
	fast_io::timer t("fast_io::ostring");
	for(std::size_t i{};i!=N;++i)
	{
		fast_io::ostring ostr;
		print(ostr,vec[i]);
		fast_io_ostr_value+=ostr.str().size();
	}
	}
	std::size_t fast_io_value{};
	{
	fast_io::timer t("fast_io::concat");
	for(std::size_t i{};i!=N;++i)
		fast_io_value+=fast_io::concat(vec[i]).size();
	}
	std::size_t fmt_value{};
	{
	fast_io::timer t("fmt::format");
	for(std::size_t i{};i!=N;++i)
		fmt_value+=fmt::format_int(vec[i]).str().size();
	}
	println("fast_io::ostring value:",fast_io_ostr_value,"\nfast_io::concat value:",fast_io_value,"\nfmt::format value:",fmt_value);
}