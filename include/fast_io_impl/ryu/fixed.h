#pragma once
#include"fixed_table.h"

namespace fast_io::details::ryu
{

template<std::floating_point T>
struct floating_traits
{
};

template<std::uint8_t base,bool uppercase,output_stream output,std::unsigned_integral U>
requires base_number_upper_constraints<base,uppercase>::value
inline constexpr void unsafe_setz_base_number(output& out,U a,std::size_t width)
{
	if constexpr(buffer_output_stream<output>)
	{
		auto reserved(oreserve(out,width));
		if constexpr(std::is_pointer_v<decltype(reserved)>)
		{
			if(reserved)
			{
				std::fill(reserved-width,output_base_number_impl<base,uppercase>(reserved,a),'0');
				return;
			}
		}
		else
		{
			std::fill(reserved-width,output_base_number_impl<base,uppercase>(reserved,a),'0');
			return;
		}
	}
	basic_ostring<std::basic_string<typename output::char_type>> bos(sizeof(a)*8,0);
	auto &v(orange(bos));
	auto const e(v.data()+v.size());
	std::fill(v.data(),output_base_number_impl<base,uppercase>(e,a),'0');
	writes(out,v.data(),e);
}

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

inline constexpr bool multiple_of_power_of_2(__uint128_t value,std::size_t p) {
// return __builtin_ctz(value) >= p;
return !(value & ((static_cast<__uint128_t>(1)<<p) - 1));
}

inline constexpr uint32_t log10_pow2(uint64_t e) {
return (uint32_t) ((((uint64_t) e) * 169464822037455ull) >> 49);
}
template<std::unsigned_integral T>
inline constexpr std::size_t length_for_index(T idx)
{return (log10_pow2(idx<<4)+25)/9;}

template<typename T>
inline constexpr uint32_t mul_shift_mod_1e9(__uint128_t m, std::array<T,3> const& mul, std::size_t j)
{
	__uint128_t const b0(m * mul[0]);
	__uint128_t const b1(m * mul[1]);
	__uint128_t const b2(m * mul[2]);
	__uint128_t const mid(b1 + (b0 >> 64));
	__uint128_t const s1(b2 + (mid >> 64));
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

template<std::floating_point floating,std::unsigned_integral mantissaType,std::signed_integral exponentType>
inline constexpr unrep<mantissaType,exponentType> init_rep(mantissaType const& mantissa,exponentType const& exponent)
{
	if(!exponent)
		return {mantissa,1-static_cast<exponentType>(floating_traits<floating>::bias+floating_traits<floating>::exponent_bits)};
	return {static_cast<mantissaType>((static_cast<mantissaType>(1)<<floating_traits<floating>::mantissa_bits)|mantissa),
		static_cast<exponentType>(exponent-static_cast<exponentType>(floating_traits<floating>::bias+floating_traits<floating>::mantissa_bits))};
}

template<std::unsigned_integral T,std::unsigned_integral E,character_output_stream output,std::floating_point F>
requires range_output_stream<output>
inline constexpr void output_fixed(output& out, F d,std::size_t precision)
{
	using signed_E = std::make_signed_t<E>;
	auto const bits(bit_cast<T>(d));
	// Decode bits into sign, mantissa, and exponent.
	bool const sign((bits >> (floating_traits<F>::mantissa_bits + floating_traits<F>::exponent_bits)) & 1u);
	T const mantissa(bits & ((static_cast<T>(1u) << floating_traits<F>::mantissa_bits) - 1u));
	E const exponent(static_cast<E>(((bits >> floating_traits<F>::mantissa_bits) & ((static_cast<E>(1u) << floating_traits<F>::exponent_bits) - 1u))));
	// Case distinction; exit early for the easy cases.
	std::size_t const start_pos(size(orange(out)));
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
	auto const r2(init_rep<F>(mantissa,static_cast<signed_E>(exponent)));
	if (sign)
		put(out,'-');
	bool const negative_r2_e(r2.e<0);
	bool nonzero(false);
	if(-52<=r2.e)
	{
		E const idx(negative_r2_e?0:index_for_exponent(static_cast<E>(r2.e)));
		signed_E const p10bitsmr2e(pow10_bits_for_index(idx)-r2.e+8);
		for(std::size_t i(length_for_index(idx));i--;)
		{
			E digits(mul_shift_mod_1e9(static_cast<__uint128_t>(r2.m<<8),fixed_pow10<>::split[fixed_pow10<>::offset[idx]+i],p10bitsmr2e));
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
		signed_E j(128+(abs_e2-(idx<<4)));
		auto const of2i(fixed_pow10<>::offset_2[idx]);
		for(;i<blocks;++i)
		{
			E p(of2i+i-mb2_idx);
			E digits(mul_shift_mod_1e9(static_cast<__uint128_t>(r2.m<<8),fixed_pow10<>::split_2[p],j));
			if (fixed_pow10<>::offset_2[idx+1]<=p)
			{
				fill_nc(out,precision-9*i,'0');
				break;
			}
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
					if(required_twos<=0||(required_twos<60&&multiple_of_power_of_2(r2.m,static_cast<E>(required_twos))))
						round_up = 2;
					else
						round_up = 1;
				}
				if(maximum)
					unsafe_setz_base_number<10,false>(out,digits,maximum);
				break;
			}
		}
		if(round_up)
		{
			auto &result(orange(out));
			std::size_t round_index(osize(out)-start_pos);
			std::size_t dot_index(0);
			while(round_index--)
			{
				auto c(result[round_index]);
				if (c == '-')
				{
					result[round_index+1] = '1';
					if(dot_index)
					{
						result[dot_index] = '0';
						result[dot_index+1] = '.';
					}
					put(out,'0');
					return;
				}
				if (c == '.')
				{
					dot_index = round_index;
					continue;
				}
				else if (c == '9')
				{
					result[round_index] = '0';
					round_up = 1;
					continue;
				}
				if (round_up==2&&(!c&1))
					return;
				result[round_index]=c+1;
				return;
			}
			result.front()='1';
			if(dot_index)
			{
				result[dot_index] = '0';
				result[dot_index+1] = '.';
			}
			put(out,'0');
		}
	}
	else
		fill_nc(out,precision,'0');
}

}