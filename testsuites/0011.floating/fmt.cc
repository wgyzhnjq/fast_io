#include<fmt/format.h>
//#include"../../include/fast_io.h"

int main()
{
	if("10"==fmt::format("{}",10))
		fmt::print("Hello I am 10\n");
	else
		fmt::print("no I am not 10\n");	
}