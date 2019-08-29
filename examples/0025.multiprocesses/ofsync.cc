#include"../../include/fast_io.h"
#include<random>

int main()
{
	std::random_device rd;
	std::size_t process_random_number(rd());
	fast_io::ofsync fsync("sync_log.txt");			//fsync will call flush in its destructor while sync won't
	for(std::size_t i(0);i!=1000000;++i)
	{
		println(fsync,"Process Random ",process_random_number," ",i);
		fsync.flush();
	}
}