//CppCon 2015: Kate Gregory “Stop Teaching C"
//https://www.youtube.com/watch?v=YnWhqhNdYyk&t=744s
#include"../../include/fast_io.h"
#include"../../include/fast_io_in.h"

int main()
{
	using std::string;
	string name;
	scan(name);
	string greeting = "Hello, " + name;
	if (name == "Kate")
		greeting += ", I know you!";
	println(greeting);
}
