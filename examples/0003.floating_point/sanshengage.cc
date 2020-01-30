#include"../../include/fast_io.h"

inline constexpr auto f(double x)
{
	constexpr double a{2.76515378324734};
	constexpr double b{0.000451422454534717};
	constexpr double c{-6.46419597451426E-09};
	constexpr double d{4.65937074104168E-14};
	return x*(x*(x*d+c)+b)+a;
}

int main()
{
//相关系数 R2：1
	println(u8"团子:",f(300));
	println(u8"凤九:",f(30000));
	println(u8"夜华:",f(50000));
	println(u8"墨渊死时白浅:",f(70000));
	println(u8"白浅被退婚:",f(90000));
	println(u8"白浅:",f(142738));
	println(u8"墨渊，少绾:",f(360000));
	println(u8"东华:",f(365000));
	println(u8"素锦:",f(70000));
}
