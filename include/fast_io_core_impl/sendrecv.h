#pragma once

namespace fast_io
{
template<details::character_output_stream_impl T,std::forward_iterator Iter>
requires (std::same_as<typename std::iterator_traits<Iter>::value_type,typename T::char_type>)
inline constexpr void define_send_by_put(T& stm,Iter b,Iter e)
{
	for(auto i(b);i!=e;++i)
		put(stm,*i);
}

template<details::character_input_stream_impl T,std::forward_iterator Iter>
requires (std::same_as<typename std::iterator_traits<Iter>::value_type,typename T::char_type>)
inline constexpr Iter define_receive_by_get(T& stm,Iter b,Iter e)
{
	for(auto i(b);i!=e;++i)
	{
		auto [ch,err](try_get(stm));
		if(err) [[unlikely]]
			return {0,true};
		*i=ch;
	}
	return e;
}

}