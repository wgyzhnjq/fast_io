#include"../../include/fast_io_legacy.h"


template<std::floating_point F>
inline constexpr void test(std::string_view literal,F f)
{
	println(fast_io::c_stdout,u8"literal:",literal,u8"->scientific:",fast_io::scientific(f),u8"\tfixed:",fast_io::fixed(f),u8"\tgeneral:",f);
}

int main()
{
	test("0.0",0.0);
	test("1.0",1.0);
	test("10.0",10.0);
	test("100.0",100.0);
	test("1000.0",1000.0);
	test("10000.0",10000.0);
	test("100000.0",100000.0);
	test("1000000.0",1000000.0);
	test("10000000.0",10000000.0);
	test("100000000.0",100000000.0);
	test("11000.0",11000.0);
	test("23535782139572189357.0",23535782139572189357.0);
	test("235357821395721893572.0",235357821395721893572.0);
	test("2353578213957218935723.0",2353578213957218935723.0);
	test("23535782139572189357234.0",23535782139572189357234.0);
	test("1e-2",1e-2);
	test("1e-4",1e-4);
	test("1.1e-5",1e-5);
	test("1e-6",1e-6);
	test("1e-8",1e-8);
	test("1.2241412412421421412412412",1.2241412412421421412412412);
}