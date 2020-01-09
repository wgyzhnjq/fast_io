#pragma once

namespace fast_io::details::ryu
{

//FUCK CONSTEXPR NOT ALLOWING ME TO USE GOTO. FUCK WG21
template<std::floating_point F,buffer_input_stream input>
inline constexpr F input_floating(input& in)
{
	using floating_trait = floating_traits<F>;
	using mantissa_type = typename floating_trait::mantissa_type;
	using exponent_type = typename floating_trait::exponent_type;
	using unsigned_char_type = std::make_unsigned_t<typename input::char_type>;
	using signed_exponent_type = std::make_signed_t<exponent_type>;
	//.(46)-48: static_cast<unsigned_char_type>(-2)
	//-(45)-48: static_cast<unsigned_char_type>(-3)
	//'E'(69)-48: 21
	//'e'(101)-48: 53
	//We do not support Shit like EBCDIC. DEATH TO IBM
	unsigned_char_type first(front_unsigned(in));
	bool const negative{first==u8'-'};
	if(negative||first==u8'+')
		first=next_unsigned(in);
	bool const zero{first==u8'0'};
	if(zero)
		while((first=next_unsigned(in))==u8'0');
	bool decimal_zero_point_phase{first==u8'.'};
	exponent_type m10digits{};
	signed_exponent_type m10e{};
	mantissa_type mantissa{};
	bool exp_phase{};
	bool decimal_point_phase{decimal_zero_point_phase};
	if(!decimal_point_phase)[[unlikely]]
	{
		for(;m10digits!=floating_trait::digits10;++m10digits)
		{
			unsigned_char_type fr(front_unsigned<2>(in));
			if(static_cast<unsigned_char_type>(fr-=u8'0')<static_cast<unsigned_char_type>(10))[[likely]]
				mantissa=mantissa*10+fr;
			else if(fr==static_cast<unsigned_char_type>(-2))
			{
				decimal_point_phase=true;
				++in;
				break;
			}
			else if(fr==static_cast<unsigned_char_type>(21)||fr==static_cast<unsigned_char_type>(53))
			{
				exp_phase=true;
				break;
			}
			else[[unlikely]]
			{
//				if(!m10digits)
//					return bit_cast<F>(static_cast<mantissa_type>(static_cast<mantissa_type>(negative) << ((sizeof(F)<<3)-1)));
				if(negative)
					return -static_cast<F>(mantissa);
				return static_cast<F>(mantissa);
			}
			++in;
		}
		if(m10digits==floating_trait::digits10)
		{
			unsigned_char_type ch(next_unsigned<2>(in)-u8'0');
			if(ch==5)
			{
				if(mantissa&1)
				{
					if(++mantissa==floating_trait::carry10)
					{
						mantissa=floating_trait::carry10/10;
						++m10e;
					}
					for(;;++m10e)
					{
						unsigned_char_type const ch(next_unsigned<2>(in)-u8'0');
						if(ch==static_cast<unsigned_char_type>(-2))
						{
							decimal_point_phase=true;
							++in;
							break;
						}
						else if(ch==21||ch==53)
						{
							exp_phase=true;
							break;
						}
						else if(9<ch)
							break;
					}
				}
				else
				{
					for(;;++m10e)
					{
						unsigned_char_type const ch(next_unsigned<2>(in)-u8'0');
						if(ch==static_cast<unsigned_char_type>(-2))
						{
							decimal_point_phase=true;
							break;
						}
						else if(ch==21||ch==53)
						{
							exp_phase=true;
							break;
						}
						else if(ch)
						{
							if(9<ch)
								break;
							++m10e;
							if(++mantissa==floating_trait::carry10)
							{
								mantissa=floating_trait::carry10/10;
								++m10e;
							}
							for(;;++m10e)
							{
								unsigned_char_type const ch(next_unsigned<2>(in)-u8'0');
								if(ch==static_cast<unsigned_char_type>(-2))
								{
									decimal_point_phase=true;
									++in;
									break;
								}
								else if(ch==21||ch==53)
								{
									exp_phase=true;
									break;
								}
								else if(9<ch)
									break;
							}
							break;
						}
					}
				}
			}
			else if(ch<10)
			{
				++m10e;
				if(5<ch)
				{
					if(++mantissa==floating_trait::carry10)
					{
						mantissa=floating_trait::carry10/10;
						++m10e;
					}
				}
				for(;;++m10e)
				{
					unsigned_char_type const ch(next_unsigned<2>(in)-u8'0');
					if(ch==static_cast<unsigned_char_type>(-2))
					{
						decimal_point_phase=true;
						++in;
						break;
					}
					else if(ch==21||ch==53)
					{
						exp_phase=true;
						break;
					}
					else if(9<ch)
						break;
				}
			}
		}
	}
	if(decimal_point_phase)
	{
		if(decimal_zero_point_phase)
		{
			unsigned_char_type ch(next_unsigned(in));
			if(ch==u8'0')
			{
				for(--m10e;(ch=next_unsigned<2>(in))==u8'0';--m10e);
				if(static_cast<unsigned_char_type>(ch-u8'1')<static_cast<unsigned_char_type>(9))
					decimal_zero_point_phase=false;
				else
					return bit_cast<F>(static_cast<mantissa_type>(static_cast<mantissa_type>(negative) << ((sizeof(F)<<3)-1)));

			}
			else if(static_cast<unsigned_char_type>(ch-u8'1')<static_cast<unsigned_char_type>(9))
				decimal_zero_point_phase=false;
			else
			{
				if(!zero)
					throw std::runtime_error("invalid input");
			}
		}
		if(!decimal_zero_point_phase)
		{
			for(;m10digits!=floating_trait::digits10;++m10digits,--m10e)
			{
				unsigned_char_type const ch(front_unsigned<2>(in)-u8'0');
				if(ch<10)[[likely]]
					mantissa=mantissa*10+ch;
				else if(ch==21||ch==53)
				{
					exp_phase=true;
					break;
				}
				else
					break;
				++in;
			}
			//FUCK CONSTEXPR NOT ALLOWING ME TO USE GOTO

			if(m10digits==floating_trait::digits10)
			{
				unsigned_char_type const ch(next_unsigned<2>(in)-u8'0');
	//rounding. 4 discard. 6 carry. 5 is complex
				if(ch==5)
				{
					if(mantissa&1)
					{
						if(++mantissa==floating_trait::carry10)
						{
							mantissa=floating_trait::carry10/10;
							++m10e;
						}
						for(;;)
						{
							unsigned_char_type const ch(next_unsigned<2>(in)-u8'0');
							if(ch==21||ch==53)
							{
								exp_phase=true;
								break;
							}
							else if(9<ch)
								break;
						}
					}
					else
					{
						for(;;)
						{
							unsigned_char_type const ch(next_unsigned<2>(in)-u8'0');
							if(ch==21||ch==53)
							{
								exp_phase=true;
								break;
							}
							else if(ch)
							{
								if(9<ch)
									break;
								if(++mantissa==floating_trait::carry10)
								{
									mantissa=floating_trait::carry10/10;
									++m10e;
								}
								for(;;)
								{
									unsigned_char_type const ch(next_unsigned<2>(in)-u8'0');
									if(ch==21||ch==53)
									{
										exp_phase=true;
										break;
									}
									else if(9<ch)
										break;
								}
								break;
							}
						}
					}
				}
				else if(ch<10)
				{
					if(5<ch)
					{
						if(++mantissa==floating_trait::carry10)
						{
							mantissa=floating_trait::carry10/10;
							++m10e;
						}
					}
					for(;;)
					{
						unsigned_char_type const ch(next_unsigned<2>(in)-u8'0');
						if(ch==21||ch==53)
						{
							exp_phase=true;
							break;
						}
						else if(9<ch)
							break;
					}
				}
				else if(ch==21||ch==53)
					exp_phase=true;
			}
		}
	}
	if(exp_phase)
	{
		unsigned_char_type exp_fr(next_unsigned(in));
		bool const exp_negative{exp_fr==u8'-'};
		if(exp_negative||exp_fr==u8'+')
			exp_fr=next_unsigned(in);
		if(9<static_cast<unsigned_char_type>(exp_fr-u8'0'))
#ifdef __cpp_exceptions
			throw std::runtime_error("invalid input");
#else
			std::terminate();
#endif
		exponent_type exp_val{};
		if(mantissa)[[likely]]
		{
			if(exp_negative)
			{
				for(;;exp_fr=next_unsigned<2>(in))
				{
					if(static_cast<unsigned_char_type>(9)<static_cast<unsigned_char_type>(exp_fr-=0x30))
						break;
					if(m10e-static_cast<signed_exponent_type>(exp_val=exp_val*10+exp_fr)<floating_trait::minimum_exp)
						return bit_cast<F>(static_cast<mantissa_type>(static_cast<mantissa_type>(negative) << ((sizeof(F)<<3)-1)));
				}
				m10e-=static_cast<signed_exponent_type>(exp_val);
			}
			else
			{
				for(;;exp_fr=next_unsigned<2>(in))
				{
					if(static_cast<unsigned_char_type>(9)<static_cast<unsigned_char_type>(exp_fr-=0x30))
						break;
					if(floating_trait::maximum_exp<m10e+static_cast<signed_exponent_type>(exp_val=exp_val*10+exp_fr))
						return bit_cast<F>((static_cast<mantissa_type>(static_cast<mantissa_type>(negative) << ((sizeof(F)<<3)-1)))
							|static_cast<mantissa_type>(static_cast<mantissa_type>(0x7ffull) << floating_trait::mantissa_bits));
				}
				m10e+=static_cast<signed_exponent_type>(exp_val);
			}
		}
		else[[unlikely]]
		{
			for(;static_cast<unsigned_char_type>(exp_fr-=0x30)<static_cast<unsigned_char_type>(10);exp_fr=next_unsigned<2>(in));
			return bit_cast<F>(static_cast<mantissa_type>(static_cast<mantissa_type>(negative) << ((sizeof(F)<<3)-1)));
		}
	}
	else
	{
		if(m10e<floating_trait::minimum_exp)
			return bit_cast<F>(static_cast<mantissa_type>(static_cast<mantissa_type>(negative) << ((sizeof(F)<<3)-1)));
		else if(floating_trait::maximum_exp<m10e)
			return bit_cast<F>((static_cast<mantissa_type>(static_cast<mantissa_type>(negative) << ((sizeof(F)<<3)-1)))
				|static_cast<mantissa_type>(static_cast<mantissa_type>(0x7ffull) << floating_trait::mantissa_bits));
	}
	bool trailing_zeros{};
	using std::log2p1;
	signed_exponent_type e2(static_cast<signed_exponent_type>(log2p1(mantissa))+m10e-(2+floating_trait::mantissa_bits));
	mantissa_type m2{};
	if(m10e<0)
	{
		auto const p5bm10e(pow5bits(-m10e));
		e2-=p5bm10e;
		m2=mul_shift(mantissa,pow5<F,true>::inv_split[-m10e],e2-m10e+p5bm10e-1+floating_trait::pow5_bitcount);
		trailing_zeros=multiple_of_power_of_5(mantissa,-m10e);
	}
	else
	{
		e2+=log2pow5(m10e);
		m2=mul_shift(mantissa,pow5<F,true>::split[m10e],e2-m10e-pow5bits(m10e)+floating_trait::pow5_bitcount);
		trailing_zeros = e2 < m10e || multiple_of_power_of_2(mantissa, e2 - m10e);
	}
	exponent_type ieee_e2(e2 + (floating_trait::bias-1) + log2p1(m2));
	if(ieee_e2<0)
		ieee_e2=0;
	signed_exponent_type shift((!ieee_e2?1:ieee_e2)-e2-(floating_trait::bias+floating_trait::mantissa_bits));
	trailing_zeros &= !(m2 & ((1L << (shift - 1)) - 1));
	bool last_removed_bit((m2>>(shift-1))&1);
	bool round_up((last_removed_bit) && (!trailing_zeros || ((m2 >> shift) & 1)));
	return bit_cast<F>(((((static_cast<mantissa_type>(negative)) << floating_trait::exponent_bits) | static_cast<mantissa_type>(ieee_e2)) << 
	floating_trait::mantissa_bits)|(((m2 >> shift) + round_up) & ((static_cast<mantissa_type>(1) << floating_trait::mantissa_bits) - 1)));
}

}