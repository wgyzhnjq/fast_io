#include"../timer.h"
#include<unistd.h>

int main()
{
	fast_io::timer tm("::write");
	for(std::size_t i{};i!=1000000;++i)
	{
		::write(10,nullptr,0);  //write to an invalid fd
	}
}