#include<fstream>
#include<cstddef>

int main()
{
	std::ofstream fout("fst.txt",std::ofstream::binary);
	std::size_t sz(100);
	fout<<'\n';
}