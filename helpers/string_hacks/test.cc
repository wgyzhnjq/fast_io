#include"../../include/fast_io.h"
#include"impl.h"

int main()
{
	std::string str;
	fast_io::details::string_hack::set_end_ptr(str,str.data()+14);
	println(str.size());
}