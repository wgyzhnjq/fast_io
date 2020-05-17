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


template<std::integral ch_type,typename T>
inline constexpr manip::lcv<ch_type,T> lcv(basic_lconv_storage<ch_type> const& t,T f){return {t,f};}

template<std::integral ch_type,std::integral int_type>
inline constexpr std::size_t print_reserve_size(print_reserve_type_t<manip::lcv<ch_type,int_type>>)
{
	constexpr std::size_t sz{(details::cal_max_uint_size<std::make_unsigned_t<int_type>,10>()+1)<<1};
	return sz;
}

template<std::contiguous_iterator caiter,std::integral ch_type,std::integral int_type>
inline constexpr caiter print_reserve_define(print_reserve_type_t<manip::lcv<ch_type,int_type>>,caiter outiter,auto ref)
{
	auto value(ref.reference);
	auto grouping{ref.storage.grouping()};
	if(grouping.empty())
		return print_reserve_define(print_reserve_type<int_type>,outiter,value);
	auto gp_cendm1{grouping.cend()-1};
	using char_type = ch_type;
	using unsigned_char_type =std::make_unsigned_t<char_type>;
	std::array<char_type,print_reserve_size(print_reserve_type<int_type>)> str;
	namespace algo_decision = 
#ifdef FAST_IO_OPTIMIZE_SIZE
		details::optimize_size
#else
		details::jiaendu
#endif
;
	auto str_iter(str.data());
	if constexpr(std::unsigned_integral<int_type>)
		str_iter+=algo_decision::output_unsigned(str.data(),value);
	else
	{
		if(value<0)
			str_iter+=algo_decision::output_unsigned(str.data(),-static_cast<std::make_unsigned_t<int_type>>(value));
		else
			str_iter+=algo_decision::output_unsigned(str.data(),static_cast<std::make_unsigned_t<int_type>>(value));
	}
	unsigned_char_type const seperator(ref.storage.thousands_sep);
	std::array<char_type,print_reserve_size(print_reserve_type<int_type>)*2> buffer;
	auto buffer_iter(buffer.data()+buffer.size());
	for(auto iter(grouping.cbegin());;)
	{
		unsigned_char_type gp{static_cast<unsigned_char_type>(*iter)};
		if(gp)[[likely]]
		{
			if(str_iter-gp<str.data())[[unlikely]]
				break;
			if(std::is_constant_evaluated())
				std::copy_n(str_iter-=gp,gp,buffer_iter-=gp);
			else
				memcpy(buffer_iter-=gp,str_iter-=gp,gp*sizeof(char_type));
			*--buffer_iter=seperator;
		}
		if(iter!=gp_cendm1)[[likely]]
			++iter;
	}
	if(std::size_t const sz(str_iter-str.data());sz)
	{
		if(std::is_constant_evaluated())
			std::copy_n(str_iter-=sz,sz,buffer_iter-=sz);
		else
			memcpy(buffer_iter-=sz,str.data(),sz*sizeof(char_type));
	}
	else
		++buffer_iter;
	if constexpr(std::signed_integral<int_type>)
	{
		if(value<0)
			*--buffer_iter=u8'-';
	}
	if(std::is_constant_evaluated())
		return std::copy(buffer_iter,buffer.data()+buffer.size(),outiter);
	else
	{
		std::size_t const sz(buffer.data()+buffer.size()-buffer_iter);
		memcpy(std::to_address(outiter),buffer_iter,sz*sizeof(char_type));
		return outiter+sz;
	}
}

/*
template<output_stream output,std::integral ch_type,typename T>
requires (std::integral<T>||std::floating_point<T>)
constexpr void print_define(output& out,manip::lcv<ch_type,T&> a)
{
//	if(a.storage.grouping().empty())
}
*/

/*
template<output_stream output,std::integral ch_type,typename T>
requires (std::integral<T>||std::floating_point<T>)
constexpr void print_define(output& out,manip::lcv<ch_type,T&> a)
{
	using value_type = std::remove_cvref_t<T>;
	if constexpr(std::integral<value_type>)
	{

		
		constexpr std::size_t mx_uint_sz{(details::cal_max_uint_size<value_type>()+5)*2};
		reverse_write(out,mx_uint_sz);
		if (std::is_constant_evaluated())
		{
			namespace algo_decision = fast_io::details::optimize_size;
			if constexpr(std::unsigned_integral<int_type>)
				return iter+algo_decision::output_unsigned(iter,i);
			else
			{
				if(i<0)
				{
					*iter=u8'-';
					++iter;
					return iter+algo_decision::output_unsigned(iter,-static_cast<std::make_unsigned_t<int_type>>(i));
				}
				else
					return iter+algo_decision::output_unsigned(iter,static_cast<std::make_unsigned_t<int_type>>(i));
			}
		}
		else
		{

			namespace algo_decision = 
	#ifdef FAST_IO_OPTIMIZE_SIZE
				details::optimize_size
	#else
				details::jiaendu
	#endif
		;
			if constexpr(std::unsigned_integral<int_type>)
				return iter+algo_decision::output_unsigned(iter,i);
			else
			{
				if(i<0)
				{
					*iter=u8'-';
					++iter;
					return iter+algo_decision::output_unsigned(iter,-static_cast<std::make_unsigned_t<int_type>>(i));
				}
				else
					return iter+algo_decision::output_unsigned(iter,static_cast<std::make_unsigned_t<int_type>>(i));
			}
//		}
	}
	else
	{
	}
}
*/
}