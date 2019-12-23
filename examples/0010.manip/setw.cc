#include"../../include/fast_io.h"

int main()
{
	fprint(fast_io::out,u8"%\n%\n",fast_io::width<5>("abc"),fast_io::width<4,false,u8'0'>("def"));
}

/*Output:
  abc
0def

*/