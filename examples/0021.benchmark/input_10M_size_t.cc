//Please run output_10M_size_t before this

#include"timer.h"
#include<fstream>
#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"
#include"../../include/fast_io_crypto.h"
#include<exception>
#include<memory>
#include<cstdio>
#include<vector>

int main()
try
{
	std::size_t constexpr N(10000000);
	std::vector<std::size_t> v(N);
	{
	cqw::timer t("std::FILE*");
	std::unique_ptr<std::FILE,decltype(fclose)*> fp(std::fopen("cfilestar.txt",u8"rb"),fclose);
	for(std::size_t i(0);i!=N;++i)
		auto const ret(fscanf(fp.get(),u8"%zu",v.data()+i));
	}
	std::vector<std::size_t> v2(v);
/*	{
	cqw::timer t("std::FILE* with 1048576 buffer size + _IOFBF (Full buffering) tag");
	std::unique_ptr<std::FILE,decltype(fclose)*> fp(std::fopen("cfilestar.txt",u8"rb"),fclose);
	auto buffer(std::make_unique<char[]>(1048576));
	setvbuf(fp.get(),buffer.get(),_IOFBF,1048576);
	for(std::size_t i(0);i!=N;++i)
		auto const ret(fscanf(fp.get(),u8"%zu",v.data()+i));
	}*/
	{
	cqw::timer t("std::ifstream");
	std::ifstream fin("cfilestar.txt",std::ifstream::binary);
	for(std::size_t i(0);i!=N;++i)
		fin>>v[i];
	}
/*	{
	cqw::timer t("stream_view for std::ifstream");
	fast_io::stream_view<std::ifstream> view("cfilestar.txt",std::ifstream::binary);
	for(std::size_t i(0);i!=N;++i)
		scan(view,v[i]);
	}*/
/*	{
	cqw::timer t("streambuf_view for std::ifstream");
	std::ifstream fin("cfilestar.txt",std::ifstream::binary);
	fast_io::streambuf_view view(fin.rdbuf());
	for(std::size_t i(0);i!=N;++i)
		scan(view,v[i]);
	}*/
	{
	cqw::timer t("ibuf");
	fast_io::ibuf ibuf("cfilestar.txt");
	for(std::size_t i(0);i!=N;++i)
		scan(ibuf,v[i]);
	}
	{
	cqw::timer t("ibuf_mutex");
	fast_io::ibuf_mutex ibuf("cfilestar.txt");
	for(std::size_t i(0);i!=N;++i)
		scan(ibuf,v[i]);
	}
	{
	cqw::timer t("ibuf text");
	fast_io::text_view<fast_io::ibuf> view("obuf_text.txt");
	for(std::size_t i(0);i!=N;++i)
		scan(view,v[i]);
	}
	{
	cqw::timer t("dynamic_buf isystem_file");
	fast_io::dynamic_buf ibuf(std::in_place_type<fast_io::isystem_file>,u8"cfilestar.txt");
	for(std::size_t i(0);i!=N;++i)
		scan(ibuf,v[i]);
	}
	{
	cqw::timer t("speck128/128");
	fast_io::crypto::basic_ictr<fast_io::ibuf, fast_io::crypto::speck::speck_enc_128_128> enc_stream(
		std::array<uint8_t, 16>{'8',u8'3',u8'3',u8'4',u8';',u8'2',u8'3',u8'4',u8'a',u8'2',u8'c',u8'4',u8']',u8'0',u8'3',u8'4'},
		std::array<uint8_t, 8>{'1',u8'2',u8'3',u8'4',u8'1',u8'2',u8'3',u8'4'},u8"speck.txt");
	for(auto & e : v)
		scan(enc_stream,e);
	}
}
catch(std::exception const& e)
{
	println(fast_io::err,e);
	return 1;
}