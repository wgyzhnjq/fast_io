#include"../timer.h"
#include<fstream>
#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"
//#include"../../include/fast_io_crypto.h"
#include"../../include/fast_io_legacy.h"
#include<exception>
#include<cmath>
#include<memory>
#include<cstdio>
#include<charconv>
#ifdef FAST_IO_TEST_FMT
#include<fmt/format.h>
#endif

int main()
#ifdef __cpp_exceptions
try
#endif
{
	std::size_t constexpr N(10000000);
	{
	fast_io::timer t("std::FILE*");
	std::unique_ptr<std::FILE,decltype(fclose)*> fp(std::fopen("cfilestar.txt","wb"),fclose);
	for(std::size_t i(0);i!=N;++i)
		fprintf(fp.get(),"%zu\n",i);
	}
	{
	fast_io::timer t("std::ofstream");
	std::ofstream fout("ofstream.txt",std::ofstream::binary);
	for(std::size_t i(0);i!=N;++i)
		fout<<i<<'\n';
	}
	{
	fast_io::timer t("std::ofstream with tricks");
	std::ofstream fout("ofstream_tricks.txt",std::ofstream::binary);
	auto &rdbuf(*fout.rdbuf());
	for(std::size_t i(0);i!=N;++i)
	{
		fout<<i;
		rdbuf.sputc('\n');
	}
	}
	{
	fast_io::timer t("std::to_chars + ofstream rdbuf tricks");
	std::ofstream fout("ofstream_to_chars_tricks.txt",std::ofstream::binary);
	auto &rdbuf(*fout.rdbuf());
	std::array<char, 50> buffer;
	for(std::size_t i(0);i!=N;++i)
	{
		auto [p,ec] = std::to_chars(buffer.data(), buffer.data() + buffer.size(),i);
		*p='\n';
		rdbuf.sputn(buffer.data(),++p-buffer.data());
	}
	}
	{
	fast_io::timer t("std::to_chars + obuf_file");
	fast_io::obuf_file obuf_file("std_to_chars_obuf_file.txt");
	std::array<char, 50> buffer;
	for(std::size_t i(0);i!=N;++i)
	{
		auto [p,ec] = std::to_chars(buffer.data(), buffer.data() + buffer.size(),i);
		*p='\n';
		write(obuf_file,buffer.data(),++p);
	}
	}
	{
	fast_io::timer t("obuf_file");
	fast_io::obuf_file obuf_file("obuf_file.txt");
	for(std::size_t i(0);i!=N;++i)
		println(obuf_file,i);
	}
#ifndef _MSC_VER
	{
	constexpr double dbN(N);
	fast_io::timer t("obuf_file floating");
	fast_io::obuf_file obuf_file("obuf_file_floating_int.txt");
	for(double i(0);i!=dbN;++i)
		println(obuf_file,i);
	}
	{
	constexpr double dbN(N);
	fast_io::timer t("obuf_file int hint");
	fast_io::obuf_file obuf_file("obuf_file_floating_int_hint.txt");
	for(double i(0);i!=dbN;++i)
		println(obuf_file,fast_io::int_hint(i));
	}
	{
	fast_io::timer t("transform_text");
	fast_io::obinary_to_text<fast_io::obuf_file> obuf_file("transform_text.txt");
	for(std::size_t i(0);i!=N;++i)
		println(obuf_file,i);
	}
	{
	fast_io::timer t("transform_ebcdic");
	fast_io::oascii_to_ebcdic<fast_io::obuf_file> obuf_file("transform_ebcdic.txt");
	for(std::size_t i(0);i!=N;++i)
		println(obuf_file,i);
	}
#endif
	{
	fast_io::timer t("u8obuf_file");
	fast_io::u8obuf_file u8obuf_file("u8obuf_file.txt");
	for(std::size_t i(0);i!=N;++i)
		println(u8obuf_file,i);
	}
	{
	fast_io::timer t("u8obuf_file");
	fast_io::u8obuf_file u8obuf_file_old("u8obuf_file_old.txt");
	for(std::size_t i(0);i!=N;++i)
		println(u8obuf_file_old,fast_io::dec(i));
	}
	{
	fast_io::timer t("c_file");
	fast_io::c_file cs_file("c_style.txt","wb");
	
	for(std::size_t i(0);i!=N;++i)
		println(cs_file,i);
	}
	{
	fast_io::timer t("c_file_unlocked");
	fast_io::c_file_unlocked cs_file("c_unlocked.txt","wb");
	for(std::size_t i(0);i!=N;++i)
		println(cs_file,i);
	}
	{
	fast_io::timer t("cpp_fout_view");
	std::ofstream fout("cpp_fout_vw.txt",std::ofstream::binary);
	fast_io::streambuf_view bfv(fout.rdbuf());
	for(std::size_t i(0);i!=N;++i)
		println(bfv,i);
	}
	{
	fast_io::timer t("obuf_file_mutex");
	fast_io::obuf_file_mutex obuf_file("obuf_file_mutex.txt");
	for(std::size_t i(0);i!=N;++i)
		println(obuf_file,i);
	}
	{
	fast_io::timer t("fbh");
	std::ofstream pf("fbh.txt",std::ofstream::binary);
	fast_io::filebuf_handle hd(pf);
	for(std::size_t i(0);i!=N;++i)
		println(hd,i);
	}
	{
		fast_io::timer t("fast_io::concat");
		fast_io::obuf_file fout("concat.txt");
		for(std::size_t i(0);i!=N;++i)
			println(fout,fast_io::concat(i));
	}
#ifdef FAST_IO_TEST_FMT
	{
		fast_io::timer t("fmt");
		std::ofstream fout("fmt.txt",std::ofstream::binary);
		for(std::size_t i(0);i!=N;++i)
			fout<<fmt::format("{}",i)<<'\n';
	}
	{
		fast_io::timer t("fmt obuf_file");
		fast_io::obuf_file fout("fmt_ob.txt");
		for(std::size_t i(0);i!=N;++i)
			println(fout,fmt::format("{}",i));
	}
#endif
}
#ifdef __cpp_exceptions
catch(std::exception const& e)
{
	println(fast_io::err,e);
	return 1;
}
#endif