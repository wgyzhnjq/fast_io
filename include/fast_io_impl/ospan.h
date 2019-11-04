#pragma once


namespace fast_io
{

template<typename T>
class basic_ospan
{
	T s;
public:
	typename T::pointer internal_pointer;
	using value_type = T;
	using char_type = typename T::value_type;
	template<typename ...Args>
	requires std::constructible_from<T,Args...>
	constexpr basic_ospan(Args&& ...args):s(std::forward<Args>(args)...),internal_pointer(s.data()){}
	constexpr auto& span()
	{
		return s;
	}
	constexpr void clear(){internal_pointer=s.data();}
};

template<typename T>
[[nodiscard]] inline constexpr auto& orange(basic_ospan<T>& ob)
{
	return ob.span();
}

template<typename T>
[[nodiscard]] inline constexpr std::size_t osize(basic_ospan<T>& ob)
{
	return ob.internal_pointer-ob.span().data();
}

template<typename T>
[[nodiscard]] inline constexpr auto oreserve(basic_ospan<T>& ob,std::size_t size)
{
	return ob.internal_pointer+=size;
}

template<typename T,std::contiguous_iterator Iter>
inline constexpr void writes(basic_ospan<T>& ob,Iter cbegin,Iter cend)
{
	using char_type = typename T::value_type;
	writes_precondition<char_type>(cbegin,cend);
	ob.internal_pointer=std::copy(static_cast<char_type const*>(static_cast<void const*>(std::to_address(cbegin))),
		static_cast<char_type const*>(static_cast<void const*>(std::to_address(cend))),ob.internal_pointer);
}
template<typename T>
inline constexpr void put(basic_ospan<T>& ob,typename T::value_type ch)
{
	*ob.internal_pointer=ch;
	++ob.internal_pointer;
}

template<typename T>
inline constexpr void flush(basic_ospan<T>&){}

template<typename T>
inline constexpr void fill_nc(basic_ospan<T>& os,std::size_t count,typename T::value_type const& ch)
{
	os.internal_pointer=std::fill_n(os.internal_pointer,ch,count);
}

template<output_stream output,typename T>
inline constexpr void print(output& out,basic_ospan<T> s)
{
	writes(out,s.span().data(),s.internal_pointer);
}

}

#ifdef __cpp_lib_span

#include<span>

namespace fast_io
{
	using ospan = basic_ospan<std::span<char>>;
}

#endif