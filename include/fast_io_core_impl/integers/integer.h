#pragma once
#include"util.h"
#include"optimize_size/impl.h"
#include"jiaendu/jiaendu.h"
#include"twodigits/impl.h"

namespace fast_io
{

template<std::integral int_type>
inline constexpr std::size_t print_reserve_size(print_reserve_type_t<int_type>)
{
	if constexpr(std::unsigned_integral<int_type>)
		return details::cal_max_uint_size<int_type>();
	else
		return details::cal_max_uint_size<std::make_unsigned_t<int_type>>()+1;
}

template<std::random_access_iterator caiter,std::integral int_type>
inline constexpr caiter print_reserve_define(print_reserve_type_t<int_type>,caiter iter,int_type i)
{
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
	}
}

}