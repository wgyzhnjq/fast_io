#pragma once

namespace fast_io
{


namespace details
{
template<output_stream output,Func func>
constexpr void reserve_write_cold_path(output& out,std::size_t required_size,Func& func)
{
	if(required_size<=1024)[[likely]]
	{
		std::array<typename output::char_type,1024> temp_buffer;
		write_all(out,temp_buffer.data(),func(iter.data()));
	}
	else
	{
		temp::temp_unique_arr_ptr<typename output::char_type> temp_buffer(required_size);
		write_all(out,temp_buffer.data(),func(iter.data()));
	}
}
}

template<output_stream output,Func func>
inline constexpr void reserve_write(output& out,std::size_t required_size,Func func)
{
	if constexpr(reserve_output_stream<output>)
	{
		auto iter{oreserve(out,required_size)};
		if constexpr(std::is_pointer_v<std::remove_cvref_t<decltype(iter)>>)
		{
			if(iter==nullptr)[[unlikely]]
				details::reserve_write_cold_path(out,required_size,func);
			else
				orelease(out,func(iter));
			
		}
		else
			orelease(out,func(iter));
	}
	else
		details::reserve_write_cold_path(out,required_size,func);
}

}