#include"../../include/fast_io.h"

int main()
{
	auto const now(std::chrono::system_clock::now());
	print("Local:\t",now,"\tChinese:\t",fast_io::local_chinese(now),"\n"
		"UTC:\t",fast_io::utc(now),"\tChinese:\t",fast_io::utc_chinese(now),"\n"
		"Unix:\t",fast_io::unix(now));
}