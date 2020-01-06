#pragma once

namespace fast_io
{

template<buffer_output_stream stm,typename func>
class otransform
{
public:
	using device_type = stm;
	using transform_function_type = func;
	using char_type = typename device_type::char_type;
	device_type device;
	transform_function_type transform_function;
};

template<buffer_output_stream output,std::contiguous_iterator Iter>
inline Iter write(otransform<>,Iter cbegin,Iter cend)
{

}

}