#include"../../include/fast_io.h"
#include"../../include/fast_io_locale.h"


int main()
{
	fast_io::c_locale loc(fast_io::c_locale_category::all);
	fast_io::c_lconv_storage stg(loc);
	println("grouping:",stg.grouping(),
	"\nnegative_sign:",stg.negative_sign(),
	"\ncurrency sign:",stg.currency_symbol());
}