#pragma once

namespace fast_io
{

template<fast_io::output_stream output,std::ranges::random_access_range range>
inline void write(output& out,range const& rg)
{
	write(out,cbegin(rg),cend(rg));
}

template<fast_io::input_stream input,std::ranges::random_access_range range>
inline void read(input& in,range& rg)
{
	read(in,begin(rg),end(rg));
}


}