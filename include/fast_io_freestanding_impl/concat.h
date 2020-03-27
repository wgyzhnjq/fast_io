#pragma once

namespace fast_io
{
//potential constexpr in the future if std::string can be constexpr


namespace details
{
template<typename T,bool ln,typename U>
inline constexpr T deal_with_one(U&& t)
{
	using value_type = typename T::value_type;
	using no_cvref = std::remove_cvref_t<U>;
	std::array<value_type,print_reserve_size(print_reserve_type<no_cvref>)+static_cast<std::size_t>(ln)> array;
	if constexpr(ln)
	{
		auto p {print_reserve_define(print_reserve_type<no_cvref>,array.data(),std::forward<U>(t))};
		*p=u8'\n';
		return T(array.data(),++p);
	}
	else
	{
		return T(array.data(),
		print_reserve_define(print_reserve_type<no_cvref>,array.data(),std::forward<U>(t)));
	}
}

template <typename T>
struct is_std_string
{
inline static constexpr bool value{};
};
//https://stackoverflow.com/questions/51862465/check-if-a-type-is-stdbasic-stringt-in-compile-time-in-c
template <typename T, typename Traits, typename Alloc>
struct is_std_string<std::basic_string<T, Traits, Alloc>>
{
inline static constexpr bool value{true};
};
}

template<typename T=std::string,typename... Args>
inline constexpr T concat(Args&& ...args)
{
	if constexpr(sizeof...(Args)==1&&(reserve_printable<Args>&...)&&details::is_std_string<T>::value)
		return details::deal_with_one<T,false>(std::forward<Args>(args)...);
	else
	{
		T v;
		basic_ostring_ref<T> t(v);
		print(t,std::forward<Args>(args)...);
		return std::move(v);
	}
}

template<typename T=std::string,typename... Args>
inline constexpr T concatln(Args&& ...args)
{
	if constexpr(sizeof...(Args)==1&&(reserve_printable<Args>&...)&&details::is_std_string<T>::value)
		return details::deal_with_one<T,true>(std::forward<Args>(args)...);
	else
	{
		T v;
		basic_ostring_ref<T> t(v);
		println(t,std::forward<Args>(args)...);
		return std::move(v);
	}
}

template<typename T=std::string,typename... Args>
inline constexpr T format(std::string_view format,Args&& ...args)
{
	T v;
	basic_ostring_ref<T> t(v);
	fprint(t,format,std::forward<Args>(args)...);
	return std::move(v);
}

template<typename T,typename... Args>
inline constexpr void in_place_to(T& t,Args&& ...args)
{
	basic_ostring<std::string> os;
	print(os,std::forward<Args>(args)...);
	basic_istring_view<std::string_view> is(os.str());
	scan(is,t);		//Todo. no_decoration
}

template<typename... Args>
inline constexpr void in_place_to(std::string& t,Args&& ...args)
{
	basic_ostring_ref<std::string> os(t);
	os.clear();
	print(os,std::forward<Args>(args)...);
}

template<typename T,typename... Args>
inline constexpr T to(Args&& ...args)
{
	if constexpr(details::is_std_string<T>::value)
		return details::deal_with_one<T,false>(std::forward<Args>(args)...);
	else
	{
		T t;
		in_place_to(t,std::forward<Args>(args)...);
		return std::move(t);
	}
}
}