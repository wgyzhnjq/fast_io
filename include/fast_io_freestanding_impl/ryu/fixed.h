#pragma once

namespace fast_io::details::ryu
{

template<std::floating_point T>
struct floating_traits
{
};


template<>	
struct floating_traits<float>
{
	using mantissa_type = std::uint32_t;
	using exponent_type = std::uint32_t;
	static inline constexpr exponent_type exponent_bits = 8;
	static inline constexpr mantissa_type mantissa_bits = sizeof(float)*8-1-exponent_bits;
	static inline constexpr exponent_type exponent_max = (static_cast<exponent_type>(1)<<exponent_bits)-1;
	static inline constexpr exponent_type bias = (static_cast<exponent_type>(1)<<(exponent_bits - 1)) - 1;
//	static inline constexpr exponent_type pow5_inv_bitcount= 61;
	static inline constexpr exponent_type pow5_bitcount= 61;
	static inline constexpr exponent_type floor_log5 = 9;
	static inline constexpr exponent_type bound = 31;//ryu to do. use a tigher bound
	static inline constexpr exponent_type digits10=8;
	static inline constexpr mantissa_type carry10=0x989680;
};

template<>	
struct floating_traits<double>
{
	using mantissa_type = std::uint64_t;
	using exponent_type = std::uint32_t;
	static inline constexpr exponent_type exponent_bits = 11;
	static inline constexpr mantissa_type mantissa_bits = sizeof(double)*8-1-exponent_bits;
	static inline constexpr exponent_type exponent_max = (static_cast<exponent_type>(1)<<exponent_bits)-1;
	static inline constexpr exponent_type bias = (static_cast<exponent_type>(1)<<(exponent_bits - 1)) - 1;
//	static inline constexpr exponent_type pow5_inv_bitcount= 125;
	static inline constexpr exponent_type pow5_bitcount= 125;
	static inline constexpr exponent_type floor_log5 = 21;
	static inline constexpr exponent_type bound = 63;//ryu to do. use a tigher bound
	static inline constexpr exponent_type digits10=17;
	static inline constexpr mantissa_type carry10=0x2386F26FC10000;
	static inline constexpr int32_t minimum_exp=-323;
	static inline constexpr int32_t maximum_exp=309;
};

template<>	
struct floating_traits<long double>
{
	using mantissa_type = uint128_t;
	using exponent_type = std::uint32_t;
	static inline constexpr exponent_type exponent_bits = 17;
	static inline constexpr std::uint32_t mantissa_bits = sizeof(long double)*8-1-exponent_bits;
	static inline constexpr exponent_type exponent_max = (static_cast<exponent_type>(1)<<exponent_bits)-1;
	static inline constexpr exponent_type bias = (static_cast<exponent_type>(1)<<(exponent_bits - 1)) - 1;
//	static inline constexpr std::size_t pow5_inv_bitcount= 249;
	static inline constexpr std::size_t pow5_bitcount= 249;
	static inline constexpr exponent_type floor_log5 = 55;
	static inline constexpr exponent_type bound = 127;//ryu to do. use a tigher bound
	static inline constexpr exponent_type digits10=21;
//	static inline constexpr mantissa_type carry10=10000000000000000ull;
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
inline constexpr bool multiple_of_power_of_2(T value,std::size_t p) {
// return __builtin_ctz(value) >= p;
return !(static_cast<uint128_t>(value) & ((static_cast<uint128_t>(1)<<p) - 1));
}

template<typename T>
inline constexpr std::uint32_t pow5_factor(T value)
{
	for (std::uint32_t count(0);value;++count)
	{
		if (value%5)
			return count;
		value/=5;
	}
	return 0;
}
template<std::integral U>
inline constexpr std::int32_t log2pow5(U e)
{
	return static_cast<int32_t>(((static_cast<std::uint32_t>(e) * 1217359) >> 19));
}

template<std::integral U>
inline constexpr std::int32_t pow5bits(U e)
{
	return log2pow5(e)+1;
}


// Returns true if value is divisible by 5^p.
template<typename T>
inline constexpr bool multiple_of_power_of_5(T value,std::uint32_t p)
{
	// The author tried a case distinction on p, but there was no performance difference.
	return p<=pow5_factor(value);
}

inline constexpr uint32_t log10_pow2(std::uint64_t e)
{
	return static_cast<std::uint32_t> (((static_cast<std::uint64_t>(e)) * 169464822037455ull) >> 49);
}
template<std::unsigned_integral T>
inline constexpr std::size_t length_for_index(T idx){return (log10_pow2(idx<<4)+25)/9;}

template<std::integral T>
inline constexpr uint32_t log10_pow5(T e)
{
	// The first value this approximation fails for is 5^2621 which is just greater than 10^1832.
	return static_cast<uint32_t> (((static_cast<uint64_t>(e)) * 196742565691928ull) >> 48);
}
/*
template<bool controller,std::unsigned_integral T>
inline constexpr std::array<fast_io::uint128_t,2> compute_pow5(T v)
{
	std::uint32_t const base(v/56);
	std::uint32_t const base2(base*56);
	std::array<std::uint64_t,4> const& mul(pow5<long double,controller>::split[base]);
	if(v==base2)
		return {construct_unsigned_extension(mul.front(),mul[1]),construct_unsigned_extension(mul[2],mul[3])};
	else
	{
		std::uint32_t const offset(v - base2);
		std::array<std::uint64_t,2> const &m = pow5<long double,controller>::[offset];
		const uint32_t delta = pow5bits(i) - pow5bits(base2);
		const uint32_t corr = (uint32_t) ((POW5_ERRORS[i / 32] >> (2 * (i % 32))) & 3);
		mul_128_256_shift(m, mul, delta, corr, result);
	}
//		return pow5<long double,controller>::;
//	pow5<long double,true>::inv_split[q]
}

template<std::unsigned_integral T>
inline constexpr std::array<fast_io::uint128_t,2> compute_pow5_inv(T v)
{
}
*/
template<std::unsigned_integral T,std::size_t muldiff=sizeof(T)*8>
requires std::same_as<T,std::uint64_t>||std::same_as<T,fast_io::uint128_t>
inline constexpr T mul_shift(T m, std::array<T,2> const& mul, std::size_t j)
{
	return low((mul_extend(m,mul.back())+high(mul_extend(m,mul.front())))>>(j-muldiff));
}

template<typename T>
requires (std::same_as<std::uint64_t,T>||std::same_as<fast_io::uint128_t,T>)
inline constexpr std::array<T,3> mul_shift_all(T m, std::array<T,2> const& mul,std::size_t j,std::uint32_t mmshift)
{
	auto const m4(m<<2);
	return {mul_shift(m4,mul,j),mul_shift(m4+2,mul,j),mul_shift(m4-1-mmshift,mul,j)};
}

template<typename T>
inline constexpr std::uint32_t mul_shift_mod_1e9(std::uint64_t m, std::array<T,3> const& mul, std::size_t j)
{
	uint128_t b1(mul_extend(m,mul[1]));
	b1+=high(mul_extend(m,mul[0]));
	uint128_t s1(mul_extend(m,mul[2]));
	s1+=high(b1);
	s1>>=j-128;
	uint128_t constexpr mulb(construct_unsigned_extension(static_cast<std::uint64_t>(0x31680A88F8953031),static_cast<std::uint64_t>(0x89705F4136B4A597)));
	return static_cast<std::uint32_t>(s1)-1000000000*static_cast<std::uint32_t>(low(mul_high(s1,mulb))>>29);
}

template<std::random_access_iterator Iter,std::unsigned_integral mantissaType>
inline constexpr auto easy_case(Iter result,bool sign, mantissaType const& mantissa)
{
	if (mantissa)
		return my_copy_n(u8"nan",3,result);
	if (sign)
		return my_copy_n(u8"-inf",4,result);
	return my_copy_n(u8"inf",3,result);
}

template<std::floating_point floating,bool ignore_exp0=false,std::unsigned_integral mantissaType,std::signed_integral exponentType>
inline constexpr unrep<mantissaType,exponentType> init_rep(mantissaType const& mantissa,exponentType const& exponent)
{
	if constexpr(!ignore_exp0)
	{
		if(!exponent)
			return {mantissa,1-static_cast<exponentType>(floating_traits<floating>::bias+floating_traits<floating>::exponent_bits)};
	}
	return {static_cast<mantissaType>((static_cast<mantissaType>(1)<<floating_traits<floating>::mantissa_bits)|mantissa),
		static_cast<exponentType>(exponent-static_cast<exponentType>(floating_traits<floating>::bias+floating_traits<floating>::mantissa_bits))};
}

template<bool uppercase_e=false,std::signed_integral T,std::random_access_iterator Iter>
requires std::same_as<T,std::int32_t>
inline constexpr Iter output_exp(T exp,Iter result)
{
	if constexpr(uppercase_e)
		*result=0x41;
	else
		*result=0x65;
	++result;
	if(exp<0)
	{
		*result=0x2d;
		++result;
		exp=-exp;
	}
	else
	{
		*result=0x2b;
		++result;
	}
	using char_type = std::remove_reference_t<decltype(*result)>;
	std::make_unsigned_t<T> unsigned_exp(exp);
	if(100<=unsigned_exp)[[unlikely]]
		return my_copy_n(jiaendu::static_tables<char_type>::table3[unsigned_exp].data(),3,result);
	return my_copy_n(jiaendu::static_tables<char_type>::table2[unsigned_exp].data(),2,result);
}

template<std::size_t precision,bool scientific = false,bool uppercase_e=false,std::random_access_iterator Iter,std::floating_point F>
inline constexpr auto output_fixed(Iter result, F d)
{
	using floating_trait = floating_traits<F>;
	using mantissa_type = typename floating_trait::mantissa_type;
	using exponent_type = typename floating_trait::exponent_type;
	using signed_exponent_type = std::make_signed_t<exponent_type>;
	using char_type = std::remove_reference_t<decltype(*result)>;
	auto const bits(bit_cast<mantissa_type>(d));
	// Decode bits into sign, mantissa, and exponent.
	bool const sign((bits >> (floating_trait::mantissa_bits + floating_trait::exponent_bits)) & 1u);
	mantissa_type const mantissa(bits & ((static_cast<mantissa_type>(1u) << floating_trait::mantissa_bits) - 1u));
	exponent_type const exponent(static_cast<exponent_type>(((bits >> floating_trait::mantissa_bits) & floating_trait::exponent_max)));
	// Case distinction; exit early for the easy cases.
	if(exponent == floating_trait::exponent_max)
		return easy_case(result,sign,mantissa);
	auto start(result);
	if(!exponent&&!mantissa)
	{
		if(sign)
		{
			*result=0x2d;
			++result;
		}
		*result=0x30;
		++result;
		if constexpr(precision!=0)
		{
			*result=0x2E;
			++result;
			result=my_fill_n(result,precision,0x30);
			if constexpr(scientific)
			{
				if constexpr(uppercase_e)
					return my_copy_n(u8"E+00",4,result);
				else
					return my_copy_n(u8"e+00",4,result);
			}
		}
		return result;
	}
	auto const r2(init_rep<F>(mantissa,static_cast<signed_exponent_type>(exponent)));
	if (sign)
	{
		*result=0x2d;
		++result;
	}
	bool const negative_r2_e(r2.e<0);
	if constexpr(scientific)
	{
		constexpr std::size_t scientific_precision(precision+1);
		exponent_type digits(0),printed_digits(0),available_digits(0);
		signed_exponent_type exp(0);
		if(-52<=r2.e)
		{
			exponent_type const idx(negative_r2_e?0:index_for_exponent(static_cast<exponent_type>(r2.e)));
			signed_exponent_type const p10bitsmr2e(pow10_bits_for_index(idx)-r2.e+8);
			auto const idx_offset(fixed_pow10<>::offset[idx]);
			for(std::size_t i(length_for_index(idx));i--;)
			{
				digits=mul_shift_mod_1e9(r2.m<<8,fixed_pow10<>::split[idx_offset+i],p10bitsmr2e);
				if(printed_digits)
				{
					if constexpr(precision<9)
					{
						available_digits=9;
						break;
					}
					else if(scientific_precision < printed_digits + 9)
					{
						available_digits=9;
						break;
					}
					my_fill(result,output_base_number_impl<10,false>(result+9,digits),0x30);
					result+=9;
					printed_digits+=9;
				}
				else if(digits)
				{
					available_digits = chars_len<10,true>(digits);
					exp = static_cast<signed_exponent_type>(i*9 + available_digits - 1);
					if(scientific_precision < available_digits)
						break;
					if constexpr (precision!=0)
						output_base_number_impl<10,false,true>(result+=available_digits+1,digits);
					else
					{
						*result=static_cast<char_type>(0x30+digits);
						++result;
					}
					printed_digits = available_digits;
					available_digits = 0;
				}
			}
		}
		if(negative_r2_e&&!available_digits)
		{
			auto abs_e2(-r2.e);
			exponent_type const idx(static_cast<exponent_type>(abs_e2)>>4);
			signed_exponent_type j(128+(abs_e2-(idx<<4)));
			exponent_type const of2i(fixed_pow10<>::offset_2[idx]);
			exponent_type const idxp1(fixed_pow10<>::offset_2[idx+1]);
			exponent_type const mb2_idx(fixed_pow10<>::min_block_2[idx]);
			for (exponent_type i(mb2_idx); i < 200; ++i)
			{
				exponent_type const p(of2i+i-mb2_idx);
				digits=(idxp1<=p)?0:mul_shift_mod_1e9(r2.m<<8,fixed_pow10<>::split_2[p],j);
				if(printed_digits)
				{
					if constexpr(precision<9)
					{
						available_digits=9;
						break;
					}
					else if(scientific_precision < printed_digits + 9)
					{
						available_digits=9;
						break;
					}
					my_fill(result,output_base_number_impl<10,false>(result+9,digits),0x30);
					result+=9;
					printed_digits+=9;
				}
				else if(digits)
				{
					available_digits=chars_len<10,true>(digits);
					exp = static_cast<signed_exponent_type> (available_digits -(i + 1) * 9 - 1);
					if (scientific_precision<available_digits)
						break;
					if constexpr (precision!=0)
						output_base_number_impl<10,false,true>(result+=available_digits+1,digits);
					else
					{
						*result=static_cast<char_type>(0x30+digits);
						++result;
					}
					printed_digits = available_digits;
					available_digits = 0;
				}
			}
		}
		exponent_type const maximum(scientific_precision - printed_digits);
		exponent_type lastdigit(0);
		for(exponent_type k(maximum);k<available_digits;++k)
		{
			lastdigit = digits%10;
			digits /= 10;
		}
		std::size_t round_up(0);
		if(lastdigit!=5)
			round_up = 5 < lastdigit;
		else
		{
			signed_exponent_type const rexp (static_cast<signed_exponent_type> (scientific_precision - exp));
			signed_exponent_type const required_twos(-r2.e - rexp);
			bool trailing_zeros(required_twos <= 0 || (required_twos < 60 && multiple_of_power_of_2(r2.m, static_cast<exponent_type>(required_twos))));
			if (rexp < 0)
			{
				signed_exponent_type required_fives = -rexp;
				trailing_zeros = trailing_zeros && multiple_of_power_of_5(r2.m, static_cast<exponent_type>(required_fives));
			}
			round_up = trailing_zeros ? 2 : 1;
		}
		if(printed_digits)
		{
			if(digits)
			{
				my_fill(result,output_base_number_impl<10,false>(result+maximum,digits),0x30);
				result+=maximum;
			}
			else
				result=my_fill_n(result,maximum,0x30);
		}
		else
		{
			if constexpr(precision!=0)
			{
				my_fill(result,output_base_number_impl<10,false,true>(result+maximum+1,digits),0x30);
				result+=maximum+1;
			}
			else
			{
				*result = 0x30 + digits;
				++result;
			}
		}
		if(round_up)
		{
			std::size_t round_index(result-start);
			while(round_index--)
			{
				auto c(start[round_index]);
				if (c == 0x2d)
				{
					start[round_index+1] = 0x31;
					++exp;
					break;
				}
				if constexpr(precision==0)
				{
					if (c == 0x39)
					{
						start[round_index] = 0x30;
						round_up = 1;
						continue;
					}
					else
					{
						if (round_up==2&&!(c&1))
							break;
						start[round_index]=c+1;
						break;
					}
				}
				else
				{
					if (c == 0x2E)
						continue;
					else if (c == 0x39)
					{
						start[round_index] = 0x30;
						round_up = 1;
						continue;
					}
					else
					{
						if (round_up==2&&!(c&1))
							break;
						start[round_index]=c+1;
						break;
					}
				}
			}
			if(round_index==static_cast<std::size_t>(-1))
			{
				start[round_index+1] = 0x31;
				++exp;
			}
		}
		return output_exp<uppercase_e>(exp,result);
	}
	else
	{
		bool nonzero(false);
		if(-52<=r2.e)
		{
			exponent_type const idx(negative_r2_e?0:index_for_exponent(static_cast<exponent_type>(r2.e)));
			signed_exponent_type const p10bitsmr2e(pow10_bits_for_index(idx)-r2.e+8);
			for(std::size_t i(length_for_index(idx));i--;)
			{
				exponent_type digits(mul_shift_mod_1e9(r2.m<<8,fixed_pow10<>::split[fixed_pow10<>::offset[idx]+i],p10bitsmr2e));
				if(nonzero)
				{
					my_fill(result,output_base_number_impl<10,false>(result+9,digits),0x30);
					result+=9;
				}
				else if(digits)
				{
					output_base_number_impl<10,false>(result+=chars_len<10,true>(digits),digits);
					nonzero = true;
				}
			}
		}
		if(!nonzero)
		{
			*result=0x30;
			++result;
		}
		if constexpr(precision!=0)
		{
			*result=0x2E;
			++result;
		}
		if(negative_r2_e)
		{
			auto abs_e2(-r2.e);
			exponent_type const idx(static_cast<exponent_type>(abs_e2)>>4);
			constexpr std::size_t blocks(precision/9+1);
			std::size_t round_up(0);
			std::size_t i(0);
			auto const mb2_idx(fixed_pow10<>::min_block_2[idx]);
			if (blocks<=mb2_idx)
			{
				i=blocks;
				result=my_fill_n(result,precision,0x30);
			}
			else if(i<mb2_idx)
				result=my_fill_n(result,9*(i=mb2_idx),0x30);
			signed_exponent_type j(128+(abs_e2-(idx<<4)));
			auto const of2i(fixed_pow10<>::offset_2[idx]);
			for(;i<blocks;++i)
			{
				exponent_type p(of2i+i-mb2_idx);
				exponent_type digits(mul_shift_mod_1e9(r2.m<<8,fixed_pow10<>::split_2[p],j));
				if (fixed_pow10<>::offset_2[idx+1]<=p)
				{
					result=my_fill_n(result,precision-9*i,0x30);
					break;
				}
				if(i+1<blocks)
				{
					my_fill(result,output_base_number_impl<10,false>(result+9,digits),0x30);
					result+=9;
				}
				else
				{
					exponent_type const maximum(precision-9*i);
					exponent_type lastdigit(0);
					for(exponent_type k(maximum);k<9;++k)
					{
						lastdigit = digits%10;
						digits /= 10;
					}
					if(lastdigit!=5)
						round_up=lastdigit>5;
					else
					{
						auto const required_twos(static_cast<signed_exponent_type>(abs_e2-precision-1));
						if(required_twos<=0||(required_twos<60&&multiple_of_power_of_2(r2.m,static_cast<exponent_type>(required_twos))))
							round_up = 2;
						else
							round_up = 1;
					}
					if(maximum)
					{
						my_fill(result,output_base_number_impl<10,false>(result+maximum,digits),0x30);
						result+=maximum;
					}
					break;
				}
			}
			if(round_up)
			{
				std::size_t round_index(result-start);
				if constexpr(precision!=0)
				{
					std::size_t dot_index(0);
					while(round_index--)
					{
						auto c(start[round_index]);
						if (c == 0x2d)
						{
							start[round_index+1] = 0x31;
							if(dot_index)
							{
								start[dot_index] = 0x30;
								start[dot_index+1] = 0x2E;
							}
							*result=0x30;
							return ++result;
						}
						if (c == 0x2E)
						{
							dot_index = round_index;
							continue;
						}
						else if (c == 0x39)
						{
							start[round_index] = 0x30;
							round_up = 1;
							continue;
						}
						if (round_up!=2||c&1)
							start[round_index]=c+1;
						return result;
					}
					*start=0x31;
					if(dot_index)
					{
						start[dot_index] = 0x30;
						start[dot_index+1] = 0x2E;
					}
				}
				else
				{
					while(round_index--)
					{
						auto c(start[round_index]);
						if (c == 0x2d)
						{
							start[round_index+1] = 0x31;
							*result=0x30;
							return ++result;
						}
						if (c == 0x39)
						{
							start[round_index] = 0x30;
							round_up = 1;
							continue;
						}
						if (round_up!=2||c&1)
							start[round_index]=c+1;
						return result;
					}
					*start=0x31;
				}
				*result=0x30;
				++result;
			}
			return result;
		}
		return my_fill_n(result,precision,0x30);
	}
}
template<std::floating_point floating,std::unsigned_integral mantissaType,std::signed_integral exponentType>
inline constexpr unrep<mantissaType,exponentType> init_repm2(mantissaType const& mantissa,exponentType const& exponent)
{
	if(!exponent)
		return {mantissa,1-static_cast<exponentType>(floating_traits<floating>::bias+floating_traits<floating>::exponent_bits+2)};
	return {static_cast<mantissaType>((static_cast<mantissaType>(1)<<floating_traits<floating>::mantissa_bits)|mantissa),
		static_cast<exponentType>(exponent-static_cast<exponentType>(floating_traits<floating>::bias+floating_traits<floating>::mantissa_bits+2))};
}

template<bool uppercase_e=false,std::size_t mode=0,bool int_hint=false,std::random_access_iterator Iter,std::floating_point F>
inline constexpr Iter output_shortest(Iter result, F d)
{
	using char_type = std::remove_reference_t<decltype(*result)>;
	using floating_trait = floating_traits<F>;
	using mantissa_type = typename floating_trait::mantissa_type;
	using exponent_type = typename floating_trait::exponent_type;
	using signed_exponent_type = std::make_signed_t<exponent_type>;
	auto const bits(bit_cast<mantissa_type>(d));
	// Decode bits into sign, mantissa, and exponent.
	bool const sign((bits >> (floating_trait::mantissa_bits + floating_trait::exponent_bits)) & 1u);
	mantissa_type const mantissa(bits & ((static_cast<mantissa_type>(1u) << floating_trait::mantissa_bits) - 1u));
	exponent_type const exponent(static_cast<exponent_type>(((bits >> floating_trait::mantissa_bits) & floating_trait::exponent_max)));
	// Case distinction; exit early for the easy cases.
	if(exponent == floating_trait::exponent_max)
		return easy_case(result,sign,mantissa);
	if(!exponent&&!mantissa)
	{
		if(sign)
		{
			*result=0x2d;
			++result;
		}
		*result=0x30;
		++result;
		if constexpr(mode==2)
		{
			if constexpr(uppercase_e)
				return my_copy_n(u8"E+00",4,result);
			else
				return my_copy_n(u8"e+00",4,result);
		}
		return result;
	}
	if constexpr(int_hint)//scientific integer hint?? Is that useless?
	{
		auto const r2(init_rep<F,false>(mantissa,static_cast<signed_exponent_type>(exponent)));
		if(-52<=r2.e&&r2.e<=0)[[likely]]
		{
			mantissa_type const mask = (static_cast<mantissa_type>(1) << -r2.e) - 1;
			if (!(r2.m & mask))[[likely]]
			{
				auto v2(r2.m>>-r2.e);
				if(sign)
				{
					*result=0x2d;
					++result;
				}
				if constexpr(mode==0)
				{
					if(v2)[[likely]]
					{
						mantissa_type m1(v2%static_cast<mantissa_type>(10000));
						mantissa_type v3(v2/static_cast<mantissa_type>(10000));
						if(m1)[[likely]]//This must be fixed form
							result+=jiaendu::output_unsigned(v2,result);
						else
						{
							for(;;)
							{
								auto const m(v3%10),d(v3/10);
								if(m)
									break;
								v3=d;
							}
							auto const v2_len(chars_len<10,true>(v2));
							auto const v3_len(chars_len<10,true>(v3));
							auto const v3_smaller_than10(v3<10);
							if(v3_len+(v3_smaller_than10?4:5)<v2_len)[[unlikely]]//scientific decision
							{
								if(v3_smaller_than10)
								{
									*result=static_cast<char_type>(v3+u8'0');
									++result;
								}
								else
								{
									jiaendu::output_unsigned(v3,result+1);
									*result=result[1];
									result[1]=u8'.';
									result+=v3_len+1;
								}
								if constexpr(uppercase_e)
									my_copy_n(u8"E+",2,result);
								else
									my_copy_n(u8"e+",2,result);
								result+=2;
								my_copy_n(jiaendu::static_tables<char_type>::table2[v2_len-1].data(),2,result);
								result+=2;
							}
							else
							{
								jiaendu::output_unsigned(v2,result);
								result+=v2_len;
							}
						}
					}
					else
					{
						*result=0x30;
						++result;
					}
				}
				else if constexpr(mode==1)		//fixed
					result+=jiaendu::output_unsigned(v2,result);
				else	//scientific
				{
					auto const v2_len(chars_len<10,true>(v2));
					for(;;)
					{
						auto const m(v2%10),d(v2/10);
						if(m)
							break;
						v2=d;
					}
					result+=output_unsigned_point(v2);
					if constexpr(uppercase_e)
						my_copy_n(u8"E+",2,result);
					else
						my_copy_n(u8"e+",2,result);
					result+=2;
					my_copy_n(jiaendu::static_tables<char_type>::table2[v2_len-1].data(),2,result);
					result+=2;
				}
				return result;
			}
		}
	}
	auto const r2(init_repm2<F>(mantissa,static_cast<signed_exponent_type>(exponent)));
	bool const accept_bounds(!(r2.m&1));
	auto const mv(r2.m<<2);
	exponent_type const mm_shift(mantissa||static_cast<signed_exponent_type>(exponent)<2);
	std::array<mantissa_type,3> v{};
	//vr,vp,vm
	signed_exponent_type e10{};
	bool vm_is_trailing_zeros{},vr_is_trailing_zeros{};
	if(0<=r2.e)
	{
		exponent_type const q(log10_pow2(r2.e)-(3<r2.e));
		e10=static_cast<signed_exponent_type>(q);
		signed_exponent_type const k(floating_trait::pow5_bitcount + pow5bits(q) - 1);
		signed_exponent_type const i(-r2.e+static_cast<signed_exponent_type>(q)+k);
		if constexpr(std::same_as<std::remove_cvref_t<F>,long double>)
			v=mul_shift_all(r2.m,compute_pow5_inv(q),i,mm_shift);
		else
			v=mul_shift_all(r2.m,pow5<F,true>::inv_split[q],i,mm_shift);
		if(q<=floating_trait::floor_log5)//here
		{
			if(!(mv%5))
				vm_is_trailing_zeros=multiple_of_power_of_5(mv,q);
			else if(accept_bounds)
				vm_is_trailing_zeros=multiple_of_power_of_5(mv-1-mm_shift,q);
			else
				v[1]-=multiple_of_power_of_5(mv+2,q);
		}
	}
	else
	{
		exponent_type abs_e2(static_cast<exponent_type>(-r2.e));
		exponent_type const q(log10_pow5(abs_e2)-(1<abs_e2));
		signed_exponent_type const signed_q(static_cast<signed_exponent_type>(q));
		e10=signed_q+r2.e;
		signed_exponent_type const i(-r2.e-signed_q);
		signed_exponent_type const k(pow5bits(i)-floating_trait::pow5_bitcount);
		signed_exponent_type const j(signed_q-k);
		if constexpr(std::same_as<std::remove_cvref_t<F>,long double>)
			v=mul_shift_all(r2.m,compute_pow5(i),j,mm_shift);
		else
			v=mul_shift_all(r2.m,pow5<F,true>::split[i],j,mm_shift);
		if(q<2)
		{
			vr_is_trailing_zeros=true;
			if(accept_bounds)
				vm_is_trailing_zeros=mm_shift==1;
			else
				--v[1];
		}
		else if(q<floating_trait::bound)
			vr_is_trailing_zeros=multiple_of_power_of_2(mv,q);
	}
	signed_exponent_type removed(0);
	char8_t last_removed_digit(0);
	if(vm_is_trailing_zeros||vr_is_trailing_zeros)
	{
		for(;;)
		{
			mantissa_type const vpdiv10(v[1]/10);
			mantissa_type const vmdiv10(v[2]/10);
			auto const vmmod10(static_cast<char8_t>(v[2]%10));
			if(vpdiv10 <= vmdiv10)
				break;
			mantissa_type const vrdiv10(v.front()/10);
			auto const vrmod10(static_cast<char8_t>(v.front()%10));
			vm_is_trailing_zeros&=!vmmod10;
			vr_is_trailing_zeros&=!last_removed_digit;
			last_removed_digit=static_cast<char8_t>(vrmod10);
			v.front()=vrdiv10;
			v[1]=vpdiv10;
			v[2]=vmdiv10;
			++removed;
		}
		if(vm_is_trailing_zeros)
			for(;;)
			{
				mantissa_type const vmdiv10(v[2]/10);
				auto const vmmod10(static_cast<char8_t>(v[2]%10));
				if(vmmod10)
					break;
				mantissa_type const vpdiv10(v[1]/10);
				mantissa_type const vrdiv10(v.front()/10);
				auto const vrmod10(v.front()%10);
				vr_is_trailing_zeros&=!last_removed_digit;
				last_removed_digit=static_cast<char8_t>(vrmod10);
				v.front()=vrdiv10;
				v[1]=vpdiv10;
				v[2]=vmdiv10;
				++removed;
			}
		if(vr_is_trailing_zeros&&last_removed_digit==5&&!(v.front()&1))
			last_removed_digit=4;
		v.front() += ((v.front()==std::get<2>(v)&&(!accept_bounds || !vm_is_trailing_zeros))|| 4 < last_removed_digit);
	}
	else
	{
		bool round_up(false);
		mantissa_type const vpdiv100(v[1]/100);
		mantissa_type const vmdiv100(v[2]/100);
		if(vmdiv100<vpdiv100)
		{
			mantissa_type const vrdiv100(v.front()/100);
			auto const vrmod100(v.front()%100);
			round_up=50<=vrmod100;
			v.front()=vrdiv100;
			v[1]=vpdiv100;
			v[2]=vmdiv100;
			removed+=2;
		}
		for (;;)
		{
			mantissa_type const vpdiv10(v[1]/10);
			mantissa_type const vmdiv10(v[2]/10);
			if(vpdiv10<=vmdiv10)
				break;
			mantissa_type const vrdiv10(v.front()/10);
			auto const vrmod10(v.front()%10);
			round_up=5<=vrmod10;
			v.front()=vrdiv10;
			v[1]=vpdiv10;
			v[2]=vmdiv10;
			++removed;
		}
		v.front()+=(v.front()==v[2]||round_up);
	}
	if(sign)
	{
		*result=0x2d;
		++result;
	}
	if constexpr(mode==0) //shortest
	{
		std::int32_t olength(static_cast<std::int32_t>(chars_len<10,true>(v.front())));	
		std::int32_t const real_exp(static_cast<std::int32_t>(e10 + removed + olength - 1));
		std::uint32_t fixed_length(0),this_case(0);
		if(olength<=real_exp)
		{
			fixed_length=real_exp+1;
			this_case=1;
		}
		else if(0<=real_exp&&real_exp<olength)
		{
			fixed_length=olength+2;
			if(olength==real_exp+1)
				--fixed_length;
			this_case=2;
		}
		else
			fixed_length=static_cast<exponent_type>(-real_exp)+olength+1;
		std::uint32_t scientific_length(olength==1?olength+3:olength+5);
		if(scientific_length<fixed_length)
		{
			result+=details::jiaendu::output_unsigned_point(v.front(),result);
			return output_exp<uppercase_e>(static_cast<std::int32_t>(real_exp),result);
		}
		switch(this_case)
		{
		case 1:
			jiaendu::output_unsigned(v.front(),result);
			result+=olength;
			return my_fill_n(result,real_exp+1-olength,0x30);
		case 2:
		{
			auto a(v.front());
			auto eposition(real_exp+1);
			if(olength==eposition)
			{
				jiaendu::output_unsigned(a,result);
				result+=olength;
			}
			else
			{
				jiaendu::output_unsigned(a,result+1);
				my_copy_n(result+1,eposition,result);
				result[eposition]=u8'.';
				result+=olength+1;
			}
			return result;
		}
		default:
			result=my_copy_n(u8"0.",2,result);
			result=my_fill_n(result,static_cast<exponent_type>(-real_exp-1),0x30);
			jiaendu::output_unsigned(v.front(),result);
			result+=olength;
			return result;
		}
	}
	else if constexpr(mode==1) //fixed
	{
		std::int32_t olength(static_cast<std::int32_t>(chars_len<10,true>(v.front())));	
		std::int32_t const real_exp(static_cast<std::int32_t>(e10 + removed + olength - 1));
		if(olength<=real_exp)
		{
			jiaendu::output_unsigned(v.front(),result);
			result+=olength;
			return my_fill_n(result,real_exp+1-olength,0x30);	
		}
		else if(0<=real_exp&&real_exp<olength)
		{
			auto a(v.front());
			auto eposition(real_exp+1);
			if(olength==eposition)
			{
				jiaendu::output_unsigned(a,result);
				result+=olength;
			}
			else
			{
				jiaendu::output_unsigned(a,result+1);
				my_copy_n(result+1,eposition,result);
				result[eposition]=u8'.';
				result+=olength+1;
			}
			return result;
		}
		else
		{
			result=my_copy_n(u8"0.",2,result);
			result=my_fill_n(result,static_cast<exponent_type>(-real_exp-1),0x30);
			jiaendu::output_unsigned(v.front(),result);
			result+=olength;
			return result;
		}
	}
	else		//scientific
	{
		auto a(v.front());
		std::int32_t real_exp(static_cast<std::int32_t>(e10 + removed - 1));
		if(a<10)
		{
			std::size_t olength(details::jiaendu::output_unsigned(a,result));
			real_exp+=olength;
			result+=olength;
		}
		else
		{
			std::size_t olength(details::jiaendu::output_unsigned(a,result+1));
			real_exp+=static_cast<std::int32_t>(olength);
			*result=result[1];
			result[1]=u8'.';
			result+=olength+1;
		}
		return output_exp<uppercase_e>(static_cast<std::int32_t>(real_exp),result);
	}
}

}
