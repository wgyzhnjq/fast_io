#include"timer.h"
#include<fstream>
#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"
#include"../../include/fast_io_crypto.h"
#include<exception>
#include<cmath>
#include<memory>
#include<cstdio>
#include<random>
#include<iomanip>

int main()
try
{
	std::size_t constexpr N(10000000);
	std::vector<double> vec;
	vec.reserve(N);
	std::random_device device;
	std::mt19937_64 eng(device());
	std::uniform_real_distribution dis(-1000.0,1000.0);
	for(std::size_t i(0);i!=N;++i)
		vec.emplace_back(dis(eng));
	{
	cqw::timer t("std::FILE*");
	std::unique_ptr<std::FILE,decltype(fclose)*> fp(std::fopen("cfilestardb.txt","wb"),fclose);
	for(std::size_t i(0);i!=N;++i)
		fprintf(fp.get(),"%.6f\n",vec[i]);
	}
	{
	cqw::timer t("std::ofstream");
	std::ofstream fout("ofstreamdb.txt",std::ofstream::binary);
	fout<<std::fixed<<std::setprecision(6);
	for(std::size_t i(0);i!=N;++i)
		fout<<vec[i]<<'\n';
	}
	{
	cqw::timer t("obuf");
	fast_io::obuf obuf("obufdb.txt");
	for(std::size_t i(0);i!=N;++i)
		println(obuf,fast_io::fixed<6>(vec[i]));
	}
	{
		cqw::timer t("obuf");
		fast_io::obuf obuf("obuf_no_extra_alloc.txt");
		fast_io::ostring ostr;
		for(std::size_t i(0);i!=N;++i)
		{
			ostr.clear();
			println(ostr,fast_io::fixed<6>(vec[i]));
			print(obuf,ostr.str());
		}
	}
#ifdef __cpp_lib_span
	{
		cqw::timer t("ospan");
		fast_io::obuf obuf("ospan.txt");
		std::array<char,1000> arr;
		for(std::size_t i(0);i!=N;++i)
		{
			fast_io::ospan ostr(arr);
			println(ostr,fast_io::fixed<6>(vec[i]));
			print(obuf,ostr);
		}
	}
#endif
	{
	cqw::timer t("obuf_mutex");
	fast_io::obuf_mutex obuf("obuf_mutexdb.txt");
	for(std::size_t i(0);i!=N;++i)
		println(obuf,fast_io::fixed<6>(vec[i]));
	}
	{
	cqw::timer t("speck128/128");
	fast_io::crypto::basic_octr<fast_io::obuf, fast_io::crypto::speck::speck_enc_128_128> enc_stream(
		std::array<uint8_t, 16>{'8','3','3','4',';','2','3','4','a','2','c','4',']','0','3','4'},
		std::array<uint8_t, 8>{'1','2','3','4','1','2','3','4'},"speckdb.txt");
	for(std::size_t i(0);i!=N;++i)
		println(enc_stream,fast_io::fixed<6>(vec[i]));
	}
}
catch(std::exception const& e)
{
	println(fast_io::err,e);
	return 1;
}
