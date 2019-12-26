#include"timer.h"
#include<fstream>
#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"
#include"../../include/fast_io_crypto.h"
#include"../../include/fast_io_legacy.h"
#include<exception>
#include<cmath>
#include<memory>
#include<cstdio>
#include<random>
#include<iomanip>
#include<charconv>

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
	cqw::timer t(u8"fprintf");
	fast_io::c_style_file cs("csfdb.txt","wb");
	auto fp(cs.native_handle());
	for(std::size_t i(0);i!=N;++i)
		fprintf(fp,"%g\n",vec[i]);
	}
	{
	cqw::timer t(u8"fprintf checked");
	fast_io::c_style_file cs("csfdb_checked.txt","wb");
	for(std::size_t i(0);i!=N;++i)
		fprintf(cs,"%g\n",vec[i]);
	}
	{
	cqw::timer t(u8"FILE* unlocked");
	fast_io::c_style_file_unlocked cs("filestarunlockeddb.txt","wb");
	for(std::size_t i(0);i!=N;++i)
		fprintf(cs,"%g\n",vec[i]);
	}
	{
	cqw::timer t(u8"FILE* unlocked checked");
	fast_io::c_style_file_unlocked cs("filestarunlockeddb_checked.txt","wb");
	for(std::size_t i(0);i!=N;++i)
		fprintf(cs,"%g\n",vec[i]);
	}
	{
	cqw::timer t(u8"ofstream");
	std::ofstream fout("ofs.txt");
	for(std::size_t i(0);i!=N;++i)
		fout<<vec[i]<<'\n';
	}
	{
	cqw::timer t(u8"ofstream tricks");
	std::ofstream fout("ofs_tricks.txt");
	auto &rdbuf(*fout.rdbuf());
	for(std::size_t i(0);i!=N;++i)
	{
		fout<<vec[i];
		rdbuf.sputc('\n');
	}
	}
	{
	cqw::timer t(u8"ofstream");
	std::ofstream fout("ofs.txt");
	for(std::size_t i(0);i!=N;++i)
		fout<<vec[i]<<'\n';
	}

	{
	cqw::timer t(u8"cstyle file");
	fast_io::c_style_file cs("csfdb1.txt","wb");
	for(std::size_t i(0);i!=N;++i)
		println(cs,vec[i]);
	}
	{
	cqw::timer t(u8"cstyle file unlocked");
	fast_io::c_style_file_unlocked cs("csfdb1.txt","wb");
	for(std::size_t i(0);i!=N;++i)
		println(cs,vec[i]);
	}
	{
	cqw::timer t(u8"dynamic obuf");
	fast_io::dynamic_stream dobuf(fast_io::obuf("dynamic_obufdb.txt"));
	for(std::size_t i(0);i!=N;++i)
		println(dobuf,vec[i]);
	}
	{
	cqw::timer t(u8"c_style_file_unlocked");
	fast_io::c_style_file_unlocked cs("csfdb2.txt","wb");
	for(std::size_t i(0);i!=N;++i)
		println(cs,vec[i]);
	}
	{
	cqw::timer t(u8"dynamic_buf");
	fast_io::dynamic_buf dobuf(fast_io::obuf("dynamic_bufd.txt"));
	for(std::size_t i(0);i!=N;++i)
		println(dobuf,vec[i]);
	}
	{
	cqw::timer t(u8"obuf");
	fast_io::obuf obuf("obufdb.txt");
	for(std::size_t i(0);i!=N;++i)
		println(obuf,vec[i]);
	}

	{
	cqw::timer t(u8"u8obuf");
	fast_io::u8obuf obuf("u8obufdb.txt");
	for(std::size_t i(0);i!=N;++i)
		println(obuf,vec[i]);
	}
	{
	cqw::timer t(u8"ommap");
	fast_io::omap om("omapdb.txt","w+");
	for(std::size_t i(0);i!=N;++i)
		println(om,vec[i]);
	}
	{
	cqw::timer t(u8"stream_view");
	std::ofstream fout("smvdb.txt",std::ofstream::binary);
	fast_io::stream_view stm_v(fout);
	for(std::size_t i(0);i!=N;++i)
		println(stm_v,vec[i]);
	}
#ifdef _MSC_VER
	{
		cqw::timer t(u8"charconv");
		fast_io::obuf obuf("charconv.txt");
		std::array<char,100> arr;
		for(std::size_t i(0);i!=N;++i)
		{
			auto [p,ec]=std::to_chars(arr.data(),arr.data()+arr.size(),vec[i]);
			*p='\n';
			send(obuf,arr.data(),++p);
		}
	}
#endif
	{
	cqw::timer t(u8"obuf_mutex");
	fast_io::obuf_mutex obuf("obuf_mutexdb.txt");
	for(std::size_t i(0);i!=N;++i)
		println(obuf,vec[i]);
	}
/*	{
	cqw::timer t(u8"speck128/128");
	fast_io::crypto::basic_octr<fast_io::obuf, fast_io::crypto::speck::speck_enc_128_128> enc_stream(
		std::array<uint8_t, 16>{u8'8',u8'3',u8'3',u8'4',u8';',u8'2',u8'3',u8'4',u8'a',u8'2',u8'c',u8'4',u8']',u8'0',u8'3',u8'4'}, "encdb.txt");
	}*/
}
catch(std::exception const& e)
{
	println(fast_io::err,e);
	return 1;
}
