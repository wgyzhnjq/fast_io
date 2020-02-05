#include"../timer.h"
#include<fmt/format.h>

int main()
{
	constexpr std::size_t N(10000000);
	std::size_t fast_io_ostr_value{};
	{
	fast_io::timer t("fast_io::ostring");
	fast_io::ostring ostr;
	for(std::size_t i{};i!=N;++i)
	{
		print(ostr,i);
		ostr.clear();
		fast_io_value+=ostr.str().size();
	}
	}
	std::size_t fast_io_value{};
	{
	fast_io::timer t("fast_io::concat");
	for(std::size_t i{};i!=N;++i)
		fast_io_value+=fast_io::concat(i).size();
	}
	std::size_t fmt_value{};
	{
	fast_io::timer t("fmt::format");
	for(std::size_t i{};i!=N;++i)
		fmt_value+=fmt::format("{}",i).size();
	}
	println("fast_io::ostring value:",fast_io_ostr_value,"\nfast_io::concat value:",fast_io_value,"\nfmt::format value:",fmt_value);
}