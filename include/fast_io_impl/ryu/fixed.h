#pragma once
#include"fixed_table.h"

namespace fast_io::ryu_details
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
//	using floating_type = floating;
	using mantissa_type = mantissaType;
	using exponent_type = exponentType;
//	inline static constexpr std::size_t base = Base;
	mantissa_type m=0;
	exponent_type e=0;
};

// I am not a native english speaker. bin/oct/dec/hex are OBJECTIVELY stupid for me.
/*
emplate<std::floating_point floating,mantissaType,exponentType>
inline constexpr auto base2_to_base10(floating_unsigned_representation<floating,mantissaType,exponentType,2> ieee)
{
	floating_unsigned_representation<floating,mantissaType,exponentType,10> rep;
	if(!ieee.exponent)
	{
		rep.mantissa = ieee.mantissa;
		rep.exponent = -1-floating_traits<floating>::bias-floating_traits<floating>::bits;
	}
	else
	{
		rep.exponent = ieee.exponent-floating_traits<floating>::bias-floating_traits<floating>::bits-2;
		rep.mantissa = static_cast<mantissaType>(1)<<floating_traits<floating>::bits|ieee.mantissa;
	}
	bool const even(rep.mantissa&1);
	mantissaType const mv(rep.mantissa<<2);
	exponentType const mmShift(ieee.mantissa||ieee.exponent<2);
	
}*/

template<std::unsigned_integral T>
inline constexpr T index_for_exponent(T e){return (e+15)>>4;}

template<std::unsigned_integral T>
inline constexpr T pow10_bits_for_index(T idx){return (idx<<4)+120;}

template<std::unsigned_integral T>
inline constexpr std::size_t length_for_index(T idx)
{return (log10_pow2(idx<<4)+25)/9;}

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
	if (exponent == ((1u << floating_traits<F>::exponent_bits) - 1u))
	{
		easy_case(out,sign,mantissa);
		return;
	}
	if (!exponent&& !mantissa)
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
	bool nonzero(false);
	if(-52<=r2.e)
	{
		E const idx(r2.e<0?0:index_for_exponent(static_cast<E>(r2.e)));
		auto const p10bitsmr2e(pow10_bits_for_index(idx)-r2.e);
		for(std::size_t i(length_for_index(idx));i--;)
		{
			E digits(mul_shift_mod_1e9(r2.m<<8,fixed_pow<>::split[fixed_pow<>::offset[idx]+i],static_cast<signed_E>(j+8)));
			if(nonzero)
			{
				//append_nine_digits
			}
		}
	}
//	return index;
}

}