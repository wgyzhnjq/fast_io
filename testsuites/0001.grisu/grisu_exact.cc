#include"../../include/fast_io.h"
#include"../../include/fast_io_in.h"

int main()
{
	double db;
	scan(db);

	println(db);
	println(fast_io::grisu_exact(db));
}