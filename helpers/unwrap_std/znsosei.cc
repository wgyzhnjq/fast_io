#include<ostream>
#include"../../include/fast_io.h"
#include"../../include/fast_io_legacy_impl/cpp/streambuf_io_observer.h"

extern "C" std::ostream& __wrap__ZNSolsEi(std::ostream& out,int v)
{
	debug_print("__wrap__ZNSolsEi called\n");
	fast_io::streambuf_io_observer iob{out.rdbuf()};
	print(iob,v);
	return out;
}

//gcc -Wall -Wl,--wrap,malloc -Wl,--wrap,free -o intl
/*
extern "C" std::ostream& __wrap_ZNSolsEi(std::ostream& out,int v)
{
	debug_print("_ZNSolsEi called\n");
	fast_io::streambuf_io_observer iob{out.rdbuf()};
	print(iob,v);
	return out;
}
*/