#include"../../include/fast_io.h"
#include"impl.h"

int main()
{
	std::string str;
	fast_io::obasic_string_ref<char> basr{str};
	print(basr,"Hello World\n");
	print(basr,fast_io::code_cvt(u"我是猪"));
	print(str);
}