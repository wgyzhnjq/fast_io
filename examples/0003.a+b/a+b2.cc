#include"../../include/fast_io.h"
#include<cstddef>

int main()
{
	println(fast_io::out,"Please input 2 numbers");
	std::int32_t a,b;		//signed integer will detect overflow
	scan(fast_io::in,a,b);
	println(fast_io::out,u8"sum of a+b = ",a+b);
}