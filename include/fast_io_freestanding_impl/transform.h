#pragma once

namespace fast_io
{

template<output_stream stm,
	typename sub,
	typename inbuffer=basic_buf_handler<typename sub::char_type,4096>,
	typename oubuffer=basic_buf_handler<typename stm::char_type>>
class otransform
{
public:
	using device_type = stm;
	using transform_subroutine_type = sub;
	using char_type = typename transform_subroutine_type::char_type;
	using inner_buffer_type = inbuffer;
	using outer_buffer_type = oubuffer;
	device_type device;
	transform_subroutine_type transform_subroutine;
	inner_buffer_type inner_buffer;
	outer_buffer_type outer_buffer;
};

template<output_stream stm,typename sub,typename inbuffer,typename oubuffer,
	std::contiguous_iterator Iter>
inline constexpr void write(otransform<stm,sub,inbuffer,oubuffer>& trans,Iter cbegin,Iter cend)
{

}

/*
take function (contiguous_iterator in_cbegin,contiguous_iterator in_cend,
		contiguous_iterator out_begin,contiguous_iterator out_end)
return in_cbegin+processed_characters
*/

}