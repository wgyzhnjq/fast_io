#pragma once
#include"fixed_table.h"

namespace fast_io::details::ryu
{

template<std::floating_point T>
struct floating_traits
{
};

template<>	
struct floating_traits<double>
{
	static inline constexpr std::size_t mantissa_bits = 52;
	static inline constexpr std::size_t exponent_bits = 11;
	static inline constexpr std::size_t bias = 1023;
	static inline constexpr std::size_t pow5_inv_bitcount= 122;
	static inline constexpr std::size_t pow5_bitcount= 121;
};

template<std::integral mantissaType,std::integral exponentType>
struct unrep
{
	using mantissa_type = mantissaType;
	using exponent_type = exponentType;
	mantissa_type m=0;
	exponent_type e=0;
};

template<std::unsigned_integral T>
inline constexpr T index_for_exponent(T e){return (e+15)>>4;}

template<std::unsigned_integral T>
inline constexpr T pow10_bits_for_index(T idx){return (idx<<4)+120;}

template<std::unsigned_integral T>
inline constexpr std::size_t length_for_index(T idx)
{return (log10_pow2(idx<<4)+25)/9;}

template<typename M,typename T>
inline constexpr uint32_t mul_shift_mod_1e9(M m, std::array<T,3> const& mul, std::size_t j)
{
	M const b0(m * mul[0]);
	M const b1(m * mul[1]);
	M const b2(m * mul[2]);
	M const mid(b1 + static_cast<T>(b0 >> 64));
	M const s1(b2 + static_cast<T>(mid >> 64));
	return (s1 >> (j - 128))%1000000000;
}

template<character_output_stream output,std::unsigned_integral mantissaType>
inline constexpr void easy_case(output& out,bool sign, mantissaType const& mantissa)
{
	if (mantissa)
	{
		print(out,"nan");
		return;
	}
	if (sign)
		put(out,'-');
	print(out,"inf");
}

template<std::floating_point floating,std::unsigned_integral mantissaType,std::integral exponentType>
inline constexpr unrep<mantissaType,exponentType> init_rep(mantissaType const& mantissa,exponentType const& exponent)
{
	if(!exponent)
		return {mantissa,-1-floating_traits<floating>::bias-floating_traits<floating>::bits};
	return {static_cast<mantissaType>(1)<<floating_traits<floating>::bits|mantissa,exponent-floating_traits<floating>::bias-floating_traits<floating>::bits-2};
}

template<character_output_stream output,std::unsigned_integral T,std::unsigned_integral E,std::floating_point F>
inline constexpr void output_fixed(output& out, F d,std::size_t precision)
{
	using signed_E = std::make_signed_t<E>;
	auto const bits(bit_cast<T>(d));
	// Decode bits into sign, mantissa, and exponent.
	bool const sign((bits >> (floating_traits<F>::mantissa_bits + floating_traits<F>::exponent_bits)) & 1u);
	T const mantissa(bits & ((static_cast<T>(1u) << floating_traits<F>::mantissa_bits) - 1u));
	E const exponent(static_cast<E>(((bits >> floating_traits<F>::mantissa_bits) & ((static_cast<E>(1u) << floating_traits<F>::exponent_bits) - 1u))));
	// Case distinction; exit early for the easy cases.
	if(exponent == ((1u << floating_traits<F>::exponent_bits) - 1u))
	{
		easy_case(out,sign,mantissa);
		return;
	}
	if(!exponent&&!mantissa)
	{
		if(sign)
			put(out,'-');
		if(precision)
		{
			put(out,'.');
			fill_nc(out,precision,'0');
		}
		return;
	}

	auto const r2(init_rep(mantissa,static_cast<signed_E>(exponent)));
	if (sign)
		put(out,'-');
	bool const negative_r2_e(r2.e<0);
	bool nonzero(false);
	if(-52<=r2.e)
	{
		E const idx(negative_r2_e?0:index_for_exponent(static_cast<E>(r2.e)));
		auto const p10bitsmr2e(pow10_bits_for_index(idx)-r2.e);
		for(std::size_t i(length_for_index(idx));i--;)
		{
			E digits(mul_shift_mod_1e9(r2.m<<8,fixed_pow10<>::split[fixed_pow10<>::offset[idx]+i],static_cast<signed_E>(p10bitsmr2e+8)));
			if(nonzero)
				unsafe_setw_base_number<10,false,9>(out,digits);
			else if(digits)
			{
				print(out,digits);
				nonzero = true;
			}
		}
	}
	if(!nonzero)
		put(out,'0');
	if(precision)
		put(out,'.');
	if(negative_r2_e)
	{
		auto abs_e2(-r2.e);
		E const idx(static_cast<E>(abs_e2)>>4);
		std::size_t const blocks(precision/9+1);
		std::size_t round_up(0);
		std::size_t i(0);
		auto const mb2_idx(fixed_pow10<>::min_block_2[idx]);
		if (blocks<=mb2_idx)
		{
			i=blocks;
			fill_nc(out,precision,'0');
		}
		else if(i<mb2_idx)
			fill_nc(out,9*(i=mb2_idx),'0');
		for(;i<blocks;++i)
		{
			signed_E j(120+(abs_e2-(idx<<4)));
			E p(fixed_pow10<>::offset_2[idx]+i-mb2_idx);
			if(p<=fixed_pow10<>::offset_2[idx+1])
			{
				fill_nc(out,precision-9*i,'0');
				break;
			}
			E digits(mul_shift_mod1e9(r2.m));
			if(i+1<blocks)
				unsafe_setw_base_number<10,false,9>(out,digits);
			else
			{
				E const maximum(precision-9*i);
				E lastdigit(0);
				for(E k(maximum);k<9;++k)
				{
					lastdigit = digits%10;
					digits /= 10;
				}
				if(lastdigit!=5)
					round_up=lastdigit>5;
				else
				{
					auto const required_twos(-static_cast<std::common_type_t<std::ptrdiff_t,signed_E>>(abs_e2+precision+1));
					bool const trailing_zeros(required_twos<=0||(required_twos<60&&multiple_of_power_of_2(r2.m,static_cast<E>(required_twos))));
				}
				if(maximum)
					print(out,digits);
				break;
			}
		}
		if(round_up)
		{
/*			std::size_t round_index(index);
			for(;round_index--;)
			{
			}
			if(round_index==static_cast<std::size_t>(-1))
			{

			}
			return;*/
		}
	}
	fill_nc(out,precision,'0');
}

}