#pragma once

namespace fast_io
{
namespace manip
{
template<std::integral ch_type,typename T>
struct lcv
{
	using manip_tag = manip_tag_t;
	basic_lconv_storage<ch_type> const& storage;
	T reference;
};
}

namespace details
{

template<std::integral char_type>
inline constexpr auto process_lcv_grouping(std::basic_string_view<char_type> const& grouping,
	char_type* str_first,char_type* str_last,
	char_type* buffer_iter,char_type seperator)
{
	using unsigned_char_type =std::make_unsigned_t<char_type>;
	auto gp_cendm1{grouping.cend()-1};
	for(auto iter(grouping.cbegin());;)
	{
		unsigned_char_type gp{static_cast<unsigned_char_type>(*iter)};
		if(gp)[[likely]]
		{
			if(str_last-gp<str_first)[[unlikely]]
				break;
			if(std::is_constant_evaluated())
				std::copy_n(str_last-=gp,gp,buffer_iter-=gp);
			else
				memcpy(buffer_iter-=gp,str_last-=gp,gp*sizeof(char_type));
			*--buffer_iter=seperator;
		}
		if(iter!=gp_cendm1)[[likely]]
			++iter;
	}
	if(std::size_t const sz(str_last-str_first);sz)
	{
		if(std::is_constant_evaluated())
			std::copy_n(str_first,sz,buffer_iter-=sz);
		else
			memcpy(buffer_iter-=sz,str_first,sz*sizeof(char_type));
	}
	else
		++buffer_iter;
	return buffer_iter;
}

template<typename Func>
constexpr std::size_t cal_lcv_size_base(Func func)
{
	constexpr std::size_t sz{(func()+2)<<1};
	return sz;
}

template<std::integral int_type,char8_t base>
constexpr std::size_t cal_lcv_integer_output_size()
{
	return cal_lcv_size_base([]()
	{
		return cal_max_uint_size<std::make_unsigned_t<int_type>,base>();
	});
}

template<std::floating_point fp_type,manip::floating_formats fm>
constexpr std::size_t cal_lcv_floating_len()
{
	return cal_lcv_size_base([]()
	{
		return cal_floating_len<fp_type,fm>();
	});
}

template<std::integral char_type,char8_t base,bool uppercase,std::contiguous_iterator caiter,typename T,std::integral int_type>
inline constexpr auto process_lcv_integer_output(caiter outiter,T const& storage,int_type value)
{
	auto grouping{storage.grouping()};
	if(grouping.empty())
		return process_integer_output<base,uppercase>(outiter,value);
	std::array<char_type,cal_max_uint_size<std::make_unsigned_t<int_type>,base>()> str;
	auto str_iter(details::process_integer_output<base,uppercase,true>(str.data(),value));
	constexpr std::size_t buffer_size{cal_lcv_integer_output_size<int_type,base>()};
	auto buffer_iter{process_lcv_grouping(grouping,str.data(),str_iter,outiter+buffer_size,storage.thousands_sep)};
	if constexpr(std::signed_integral<int_type>)
	{
		if(value<0)
			*--buffer_iter=u8'-';
	}
	return details::my_copy(buffer_iter,outiter+buffer_size,outiter);
}
/*
template<std::integral char_type,std::contiguous_iterator caiter,typename T>
inline constexpr auto process_lcv_floating_output(caiter outiter,T const& storage)
{
	std::array<char_type,>
	bool const minus{*outiter==u8'-'};
	if(minus)
		++begiter;
	return outiter;
}
*/
}
template<std::integral ch_type,typename T>
inline constexpr manip::lcv<ch_type,T&> lcv(basic_lconv_storage<ch_type> const& t,T&& f){return {t,f};}

template<std::integral ch_type,std::integral int_type>
inline constexpr std::size_t print_reserve_size(print_reserve_type_t<manip::lcv<ch_type,int_type&>>)
{
	return details::cal_lcv_integer_output_size<int_type,10>();
}

template<std::contiguous_iterator caiter,std::integral char_type,std::integral int_type>
inline constexpr caiter print_reserve_define(print_reserve_type_t<manip::lcv<char_type,int_type&>>,caiter outiter,auto ref)
{
	return details::process_lcv_integer_output<char_type,10,false>(outiter,ref.storage,ref.reference);
}

template<std::integral ch_type,char8_t base,bool uppercase,std::integral int_type>
inline constexpr std::size_t print_reserve_size(print_reserve_type_t<manip::lcv<ch_type,manip::base_t<base,uppercase,int_type>&>>)
{
	return details::cal_lcv_integer_output_size<int_type,base>();
}

template<std::contiguous_iterator caiter,std::integral char_type,char8_t base,bool uppercase,std::integral int_type>
inline constexpr caiter print_reserve_define(print_reserve_type_t<manip::lcv<char_type,manip::base_t<base,uppercase,int_type>&>>,caiter outiter,auto ref)
{
	return details::process_lcv_integer_output<char_type,base,uppercase>(outiter,ref.storage,ref.reference.reference);
}


/*
template<manip::floating_formats fm,bool uppercase,std::floating_point T,char32_t dec>
inline constexpr std::size_t print_reserve_size
	(print_reserve_type_t<manip::decimal_point<manip::floating_manip<fm,uppercase,T const>,dec>>)
{
	if constexpr(fm==manip::floating_formats::general||fm==manip::floating_formats::scientific)
		return 60;
	else if constexpr(fm==manip::floating_formats::fixed)
		return 700;
}

template<std::random_access_iterator raiter,manip::floating_formats fm,std::floating_point T,bool uppercase,char32_t dec,typename U>
requires (dec<std::numeric_limits<std::iter_value_t<raiter>>::max())
inline raiter print_reserve_define(print_reserve_type_t<manip::decimal_point<manip::floating_manip<fm,uppercase,T const>,dec>>,raiter start,U a)
{
	if constexpr(fm==manip::floating_formats::general)
		return details::ryu::output_shortest<dec,uppercase,0,true>(start,static_cast<double>(a.value.reference));
	else if constexpr(fm==manip::floating_formats::fixed)
		return details::ryu::output_shortest<dec,false,1,true>(start,static_cast<double>(a.value.reference));
	else if constexpr(fm==manip::floating_formats::scientific)
		return details::ryu::output_shortest<dec,uppercase,2,true>(start,static_cast<double>(a.value.reference));
}


template<std::integral char_type,std::floating_point T,char32_t dec>
inline constexpr std::size_t print_reserve_size(print_reserve_type_t<manip::lcv<char_type,manip::decimal_point<T&,dec>>>)
{
	return details::cal_lcv_floating_len<T,fm==manip::floating_formats::general>();
}

template<std::contiguous_access_iterator raiter,std::integral char_type,std::floating_point T,char32_t dec,typename U>
requires (dec<std::numeric_limits<std::iter_value_t<raiter>>::max())
inline raiter print_reserve_define(print_reserve_type_t<manip::lcv<char_type,manip::decimal_point<T&,dec>>>,raiter start,U a)
{
	return process_lcv_floating_output<char_type>(start,details::ryu::output_shortest<dec,false,0,true>(start,static_cast<double>(a.reference)),a.storage);
}
*/

}