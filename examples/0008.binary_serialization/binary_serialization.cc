#include<iostream>
#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"
#include<string>
#include<array>
#include<vector>
#include<deque>

int main()
{
	{
	fast_io::obuf out(u8"binary_serialization.txt");
	std::vector<std::vector<std::array<std::size_t,3>>> arr{std::vector<std::array<std::size_t,3>>{std::array<std::size_t,3>{12142141,2,3},std::array<std::size_t,3>{4,5,6}}};
	write(out,arr,u8"sdgsdg");
	std::array<std::u8string,1> ar{u8"abcdef\n dsjfgoisdjgoisdjgdiso"};
	write(out,ar);
	}
	{
	fast_io::ibuf_mutex in(u8"binary_serialization.txt");
	std::deque<std::vector<std::array<std::size_t,3>>> arr;
	std::u8string str;
	read(in,arr,str);
	for(auto const& e : arr)
		for(auto const& e1:e)
			for(auto const & e2 : e1)
				println(fast_io::out,e2);

	println(fast_io::out,str);
	}
}