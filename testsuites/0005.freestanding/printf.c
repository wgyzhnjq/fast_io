#include<stdio.h>
#include<stdlib.h>

int main()
{
	if(printf("Hello World\n")<0)
		abort();
}

// gcc -o hw hw.c -Ofast -flto -Ofast -DNDEBUG
// no static linking
//binary size: 18kb