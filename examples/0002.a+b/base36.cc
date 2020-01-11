#include"../../include/fast_io.h"
#include"../../include/fast_io_in.h"

int main()
{
	std::size_t a,b;
	scan(fast_io::base<36>(a),fast_io::base<36>(b));
	println(fast_io::base<36>(a+b));
}