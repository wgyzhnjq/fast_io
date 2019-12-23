#include"../../include/fast_io.h"

inline constexpr void output_floating(double const dd)
{
	union
	{
		double d;
		std::uint64_t u;
	}k{dd};
	bool sign(k.u>>63);
	std::uint64_t exponent((k.u<<static_cast<std::uint64_t>(1))>>static_cast<std::uint64_t>(53));
	std::uint64_t constexpr fractionfield((static_cast<std::uint64_t>(1)<<static_cast<std::uint64_t>(52))-static_cast<std::uint64_t>(1));
	std::uint64_t fraction(k.u&fractionfield);
	println(fast_io::out,u8"u:",fast_io::setw(64,fast_io::bin(k.u),u8'0'));
	println(fast_io::out,u8"sign:",sign);
	println(fast_io::out,u8"exponent:",exponent,u8" exponent:",fast_io::setw(11,fast_io::bin(exponent),u8'0'));
	println(fast_io::out,u8"fraction:",fraction,u8" bin:",fast_io::setw(52,fast_io::bin(fraction),u8'0'));
	if(1023<=exponent)
	{
		std::uint64_t ep(exponent-1023);
		std::uint64_t intpart((fraction>>(52-ep))|(1<<ep));
		println(fast_io::out,u8"real exponent:",ep);
		println(fast_io::out,u8"intpart:",intpart);
	}
}

//1111111100

int main()
{
	output_floating(524124124);
}