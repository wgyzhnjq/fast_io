#pragma once

namespace fast_io
{

template<buffer_output_stream output,std::integral I>
inline constexpr decltype(auto) oreserve(output& out,I n)->typename output::char_type*
{
	decltype(auto) curr{obuffer_curr(out)};
	if(obuffer_cend(out)-curr<n)[[unlikely]]
		return nullptr;
	return std::to_address(curr);
}

template<buffer_output_stream output>
inline constexpr void orelease(output& out,typename output::char_type* ptr)
{
	obuffer_curr(out)=ptr;
}

/*
inline constexpr std::size_t print_reserve_size()
{}

inline constexpr void print_reserve_define()
{}
*/
template<reserve_output_stream output,reserve_printable res>
inline constexpr void print_define(output& out,res const& t)
{
	using char_type = output::char_type;
	constexpr std::size_t size{print_reserve_size(t)};
	if constexpr(decltype(std::is_pointer_v<std::remove_cvref_t<decltype(oreserve(out))>>))
	{
		auto ptr{oreserve(out,size)};
		if(ptr==nullptr)[[unlikely]]
		{
			std::array<char_type,size> array;
			write(out,arr.data(),print_reserve_define(arr.data(),t));
			return;
		}
		orelease(out,print_reserve_define(arr.data(),t));
	}
	else
	{
		orelease(out,print_reserve_define(t,oreserve(out,size)));
	}
}

}