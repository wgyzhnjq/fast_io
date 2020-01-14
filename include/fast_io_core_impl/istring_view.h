#pragma once


namespace fast_io
{

template< typename T>
class basic_istring_view
{
	T s;
public:
	using char_type = typename T::value_type;
	template<typename ...Args>
	requires std::constructible_from<T,Args...>
	constexpr basic_istring_view(Args&& ...args):s(std::forward<Args>(args)...){}
	constexpr auto& str()
	{
		return s;
	}
	constexpr auto empty() const {return s.empty();}
};

template<typename T>
[[nodiscard]] inline constexpr auto begin(basic_istring_view<T>& isv)
{
	return isv.str().begin();
}

template<typename T>
[[nodiscard]] inline constexpr auto end(basic_istring_view<T>& isv)
{
	return isv.str().end();
}

template<typename T>
inline constexpr bool iflush(basic_istring_view<T>& is)
{
	return false;
}

template<typename T>
inline constexpr void iclear(basic_istring_view<T>& isv)
{
	return isv.str().clear();
}

template<typename T,std::integral I>
inline constexpr void operator+=(basic_istring_view<T>& isv,I i)
{
	isv.str().remove_prefix(i);
}

template<typename T>
inline constexpr basic_istring_view<T>& operator++(basic_istring_view<T>& isv)
{
	isv.str().remove_prefix(1);
	return isv;
}

template<typename T,std::contiguous_iterator Iter>
requires (sizeof(typename T::value_type)==1||
	std::same_as<typename T::value_type,typename std::iterator_traits<Iter>::value_type>)
inline constexpr Iter read(basic_istring_view<T>& istrvw,Iter begin,Iter end)
{
	using char_type = typename T::value_type;
	using iter_value_type = typename std::iterator_traits<Iter>::value_type;
	if constexpr(std::same_as<char_type,iter_value_type>)
	{
		std::size_t const cped(istrvw.str().copy(std::to_address(begin),
			std::to_address(end)-std::to_address(begin)));
		istrvw.str().remove_prefix(cped);
		return begin+cped;
	}
	else
	{
		auto pb(std::to_address(begin));
		auto pe(std::to_address(end));
		std::size_t const bytes((pe-pb)*sizeof(iter_value_type));
		if(istrvw.str().size()<bytes)
		{
			std::size_t const copied(istrvw.str().size()/sizeof(iter_value_type));
			std::memcpy(pb,istrvw.str().data(),copied*sizeof(iter_value_type));
			istrvw.str()={};
			return begin+copied;	
		}
		else
		{
			std::memcpy(pb,istrvw.str().data(),bytes);
			istrvw.str().remove_prefix(bytes);
			return end;
		}
	}

}

using u8istring_view = basic_istring_view<std::u8string_view>;
using istring_view = basic_istring_view<std::string_view>;

}
