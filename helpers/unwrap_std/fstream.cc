#include<fstream>

int main()
{
	std::ofstream fout("fst.txt",std::ofstream::binary);
	fout<<3;
}