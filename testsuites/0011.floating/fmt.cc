#include<fmt/format.h>
#include"../../include/fast_io.h"

int main()
{
	if(10==fast_io::to<int>("10"))
		print("Hello I am 10\n");
	else
		print("no I am not 10\n");	
}