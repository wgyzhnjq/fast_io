#include"../../include/fast_io.h"

/*
I wasn't thought to add shit like this. However, fmt library author keeps using stupid benchmarks to claim he is better than every else.
This print_reserver will defeat every excuse he has.
*/

int main()
{
	double d(435.4362346);
	auto p{fast_io::print_reserve(d)};
	println(fast_io::chvw(p.c_str()));
}