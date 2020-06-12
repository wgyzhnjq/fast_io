#include"../../include/fast_io.h"

int main()
{
	fast_io::wc_io_observer wstdout{stdout};
	println(wstdout,fast_io::code_cvt("hello world\n"));
	print(fast_io::code_cvt(L"hello world\n"));
}
