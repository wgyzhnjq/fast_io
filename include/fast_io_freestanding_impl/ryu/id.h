#pragma once

namespace fast_io::details::ryu
{

template<std::floating_point F,character_input_stream input>
inline constexpr F input_floating(input& in)
{
	using floating_trait = floating_traits<F>;
	using mantissa_type = typename floating_trait::mantissa_type;
	using exponent_type = typename floating_trait::exponent_type;
	using unsigned_char_type = std::make_unsigned_t<std::remove_reference_t<decltype(get(in))>>;
	mantissa_type ipart{};
	bool negative{};
	bool phase2{};
	//'.'(46)-48: static_cast<unsigned_char_type>(-2)
	//'-'(45)-48: static_cast<unsigned_char_type>(-3)
	//'E'(69)-48: 21
	//'e'(101)-48: 53
	//We do not support Shit like EBCDIC. DEATH TO IBM
	exponent_type m10digits{};
	for(;;)
	{
		unsigned_char_type ch{static_cast<unsigned_char_type>(get(in)-48};
		if(ch<10)
		{
			ipart=ch;
			++m10digits;
			break;
		}
		else
		{
			
			if(ch==static_cast<unsigned_char_type>(-3))
			{
				if((ch=static_cast<unsigned_char_type>(get(in))-48)<10)
				{
					negative=true;
					ipart=ch;
					++m10digits;
					break;
				}
				else if(ch==static_cast<unsigned_char_type>(-2))
				{
					negative=true;
					phase2=true;
					ipart={};
					break;
				}
			}
			else if(ch==static_cast<unsigned_char_type>(-2))
			{
				if((ch=static_cast<unsigned_char_type>(get(in))-48)<10)
				{
					phase2=true;
					ipart={};
					break;
				}
			}
		}
	}
	if(!phase2)
	{
		for(;m10digits<=floating_traits::digits10;++m10digits)
		{
			unsigned_char_type const ch(static_cast<unsigned_char_type>(try_get(in).first)-48);
			if(ch<10)
				ipart=ipart*10+ch;
			else if(ch==static_cast<unsigned_char_type>(-2))
			{
				phase2 = true;
				break;
			}
			else if(ch==21||ch==53)
			{
				phase2 = false;
				break;
			}
			else
			{
				if(negative)
					return -static_cast<F>(t);
				return static_cast<F>(t);
			}
		}
		if(m10digits<floating_traits::digits10)
		{

		}

	}

}

}