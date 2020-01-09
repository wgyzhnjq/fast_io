#include<iostream>
#include"../../include/fast_io.h"

int main()
{
	double df,df1;
	scan(fast_io::in,df,df1);
	println(df," ",fast_io::scientific<5>(df1));
}