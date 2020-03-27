#include"../timer.h"
#include<fmt/format.h>

int main()
{
	constexpr std::size_t N(10000000);
	std::size_t fast_io_value{};
	{
	fast_io::timer t("fast_io::ospan");
	for(std::size_t i{};i!=N;++i)
	{
		std::array<char,50> a;
		fast_io::ospan os(a);
		print(os,i);
		fast_io_value+=os.internal_pointer-a.data();
	}
	}
	std::size_t fmt_value{};
	{
	fast_io::timer t("fmt::format");
	for(std::size_t i{};i!=N;++i)
	{
		fmt_value+=fmt::format_int(i).size();
	}
	}
	println("fast_io::concat value:",fast_io_value,"\nfmt::format value:",fmt_value);
}