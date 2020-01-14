#include<fstream>
#include"../../include/fast_io_legacy.h"

int main()
{
	std::ofstream fout("w.txt");
	fast_io::filebuf_handle hd(fout);
	println(hd,"wfejioewfjeowifjio");
	fout<<"dsgdsg\n";
}