#pragma once

namespace fast_io
{

template<buffer_output_stream output,std::integral I>
inline constexpr auto oreserve(output& out,I n)->typename output::char_type*
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

template<reserve_printable T,std::integral ch>
inline constexpr std::size_t print_reserve_size(manip::follow_character<T const,ch> ref)
{
	return print_reserve_size(ref.reference)+1;
}

template<std::random_access_iterator RAiter,reserve_printable T,std::integral ch>
inline constexpr void print_reserve_define(RAiter it,manip::follow_character<T const,ch> ref)
{
	it=print_reserve_define(it,ref.reference);
	*it=ref.character;
	return ++it;
}

template<character_output_stream output,typename T,std::integral ch_type>
inline constexpr void print_define(output& out,manip::follow_character<T const,ch_type> ref)
{
	print_define(out,ref.reference);
	put(out,ref.character);
}
*/
}
