#include"timer.h"
#include<fstream>
#include"../include/fast_io.h"
#include"../include/fast_io_device.h"
//#include"../../include/fast_io_crypto.h"
#include"../include/fast_io_legacy.h"
#include<exception>
#include<cmath>
#include<memory>
#include<cstdio>
#include<charconv>

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
	fast_io::timer t("c_style_file");
	fast_io::c_style_file cs_file("c_style.txt","wb");
	
	for(std::size_t i(0);i!=N;++i)
		println(cs_file,i);
	}
	{
	fast_io::timer t("c_style_file_unlocked");
	fast_io::c_style_file_unlocked cs_file("c_style_unlocked.txt","wb");
	for(std::size_t i(0);i!=N;++i)
		println(cs_file,i);
	}
/*	{
	fast_io::timer t("cpp_fout_view");
	std::ofstream fout("cpp_fout_vw.txt",std::ofstream::binary);
	fast_io::streambuf_view bfv(fout.rdbuf());
	for(std::size_t i(0);i!=N;++i)
		println(bfv,i);
	}
	{
	fast_io::timer t("cpp_fout");
	std::ofstream fout("cpp_fout.txt",std::ofstream::binary);
	fast_io::stream_view stm_vw(fout);
	for(std::size_t i(0);i!=N;++i)
		println(stm_vw,i);
	}
	{
	fast_io::timer t("obuf_file ucs_view");
	fast_io::ucs<fast_io::obuf_file,char32_t> uv("obuf_file_ucsview.txt");
	for(std::size_t i(0);i!=N;++i)
		println(uv,i);
	}
	{
	fast_io::timer t("dynamic obuf_file");
	fast_io::dynamic_stream dobuf_file(fast_io::obuf_file("dynamic_obuf_file.txt"));
	for(std::size_t i(0);i!=N;++i)
		println(dobuf_file,i);
	}
	{
	fast_io::timer t("iobuf_file_dynamic native_file");
	fast_io::dynamic_buf dobuf_file(std::in_place_type<fast_io::onative_file>,"iobuf_file_dynamic_native_file.txt");
	for(std::size_t i(0);i!=N;++i)
		println(dobuf_file,i);
	}
	{
	fast_io::timer t("obuf_file_mutex");
	fast_io::obuf_file_mutex obuf_file("obuf_file_mutex.txt");
	for(std::size_t i(0);i!=N;++i)
		println(obuf_file,i);
	}
	{
	fast_io::timer t("fsync");
	fast_io::fsync obuf_file("fsync.txt",fast_io::open::interface<fast_io::open::out|fast_io::open::trunc>);
	for(std::size_t i(0);i!=N;++i)
		println(obuf_file,i);
	}
	{
	fast_io::timer t("speck128/128");
	fast_io::crypto::basic_octr<fast_io::obuf_file, fast_io::crypto::speck::speck_enc_128_128> enc_stream(
		std::array<uint8_t, 16>{'8',u8'3',u8'3',u8'4',u8';',u8'2',u8'3',u8'4',u8'a',u8'2',u8'c',u8'4',u8']',u8'0',u8'3',u8'4'},
		std::array<uint8_t, 8>{'1',u8'2',u8'3',u8'4',u8'1',u8'2',u8'3',u8'4'},"speck.txt");
	for(std::size_t i(0);i!=N;++i)
		println(enc_stream,i);
	}*/
}
#ifdef __cpp_exceptions
catch(std::exception const& e)
{
	println(fast_io::err,e);
	return 1;
}
#endif