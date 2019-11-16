#pragma once

namespace battlenet
{

struct key_info
{
	std::uint64_t time={};
	std::string serial;
	std::string secret_key;
};

template<fast_io::character_output_stream output>
inline constexpr void write_define(output& out,key_info const& k)
{
	write(out,k.time);
	write(out,k.serial);
	write(out,k.secret_key);
}

template<fast_io::character_input_stream input>
inline constexpr void read_define(input& in,key_info& k)
{
	read(in,k.time);
	read(in,k.serial);
	read(in,k.secret_key);
}

template<fast_io::character_output_stream output>
inline constexpr void print_define(output& out,key_info const& k)
{
	print(out,"Time: ",k.time,"\tSerial:",k.serial,"\tKey:",k.secret_key);
}

}