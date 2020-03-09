#include"../../include/fast_io.h"

inline fast_io::generator<std::string_view> gen()
{
	using namespace std::string_view_literals;
	co_yield "Hello World\n"sv;
}

int main()
{
	for(auto const& sv : gen())
	{
		print(sv);
		break;
	}
}