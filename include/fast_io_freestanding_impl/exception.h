#pragma once

namespace fast_io
{

template<output_stream output>
inline void print(output& out,std::exception const &e)
{
	print(out,e.what());
}

template<output_stream output>
inline void print(output& out,std::system_error const &e)
{
	auto const& code(e.code());
	print(out,"std::system_error, value:",code.value(),"\tmessage:",code.message());
}

}