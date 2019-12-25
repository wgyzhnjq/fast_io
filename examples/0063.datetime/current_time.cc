#include"../../include/fast_io.h"

int main()
{
	auto const now(std::chrono::system_clock::now());
	fprint(fast_io::out,"Local:\t%\nUTC:\t%\nUnix:\t%\n",now,fast_io::utc(now),fast_io::unix(now));
}