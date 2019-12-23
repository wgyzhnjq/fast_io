#include"../../include/fast_io.h"
#include<cstdint>

int main()
{
	print(fast_io::out,u8"Please input 2 hex numbers\n");
	std::int64_t a,b;
	scan(fast_io::in,fast_io::hex(a),fast_io::hex(b));
	fprint(fast_io::out,u8"sum of %(hex:%)+%(hex:%) = %(hex:%)\n",a,fast_io::hex(a),b,fast_io::hex(b)
						,a+b,fast_io::hex(a+b));
}