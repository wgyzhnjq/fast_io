#include"../../include/fast_io.h"

int main(int argc,char** argv)
{
	println(fast_io::out,u8"u8device");//pass
	println(fast_io::out,fast_io::from_utf8_unchecked(*argv));
//deal with char* to u8 strings
//learn from Rust*argv);//pass
	println(fast_io::out,"device");//error
}