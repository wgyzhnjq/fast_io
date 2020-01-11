#include"../../include/fast_io.h"
#include"../../include/fast_io_in.h"
#include<cstddef>

int main()
{
	println("Please input 2 numbers");
	std::size_t a,b;
	scan(a,b);
	println("sum of a+b = ",a+b);
}