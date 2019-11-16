#pragma once
#include<ctime>

namespace battlenet
{

struct key_info
{
	std::uint64_t time={};
	std::string serial;
	std::vector<std::uint8_t> secret_key;
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

template<std::size_t bas,bool uppercase,fast_io::buffer_output_stream output,typename T>
requires std::same_as<key_info,std::remove_cvref_t<T>>
inline constexpr void print_define(output& out,fast_io::manip::base_t<bas,uppercase,T> ref)
{
	print(out,"Unix Time:",ref.reference.time,"\tSerial:",ref.reference.serial,"\tKey:",fast_io::base_split<bas,uppercase>(ref.reference.secret_key,','));
}


template<fast_io::buffer_output_stream output>
inline constexpr void print_define(output& out,key_info const& k)
{
	print_define(out,fast_io::dec(k));
}

}