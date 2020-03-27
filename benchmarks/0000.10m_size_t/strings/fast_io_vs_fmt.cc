#include"../../timer.h"
#include<fmt/format.h>

int main()
{
	constexpr std::uint32_t N(10000000);
	std::size_t fast_io_ospan_value{};
	{
	fast_io::timer t("fast_io::ospan");
	for(std::size_t i{};i!=N;++i)
	{
		std::array<char,50> a;
		fast_io::ospan os(a);
		print(os,i);
		fast_io_ospan_value+=std::string(a.data(),os.internal_pointer).size();
	}
	}
	std::size_t fast_io_ostr_value{};
	{
	fast_io::timer t("fast_io::ostring");
	fast_io::ostring ostr;
	ostr.str().reserve(42);
	for(std::uint32_t i{};i!=N;++i)
	{
		print(ostr,i);
		fast_io_ostr_value+=ostr.str().size();
		ostr.clear();
	}
	}
	std::size_t fast_io_value{};
	{
	fast_io::timer t("fast_io::concat");
	for(std::uint32_t i{};i!=N;++i)
		fast_io_value+=fast_io::concat(i).size();
	}
	std::size_t fmt_value{};
	{
	fast_io::timer t("fmt::format");
	for(std::uint32_t i{};i!=N;++i)
		fmt_value+=fmt::format_int(i).str().size();
	}
	println("fast_io::ospan value:",fast_io_ospan_value,"\nfast_io::ostring value:",fast_io_ostr_value,"\nfast_io::concat value:",fast_io_value,"\nfmt::format value:",fmt_value);
}