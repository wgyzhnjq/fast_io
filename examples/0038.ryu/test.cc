#include"../../include/fast_io.h"

int main()
{
	fast_io::details::ryu::output_fixed<std::uint64_t,std::uint32_t>(fast_io::out,3.1,10);
//	println(fast_io::out,fast_io::ryu_details::fixed_pow10<>::split.size());
}