#pragma once

namespace fast_io
{
namespace details
{
struct is_none_space
{
template<std::integral T>
inline constexpr bool operator()(T ch)
{
	std::make_unsigned_t<T> e(ch);
	if(e==0x20)
		return false;
	return 0x4<(e-0x9);
}
};

template<bool sign,std::uint8_t base>
requires (0x2<base&&base<=0x36)
struct is_numerical
{
template<std::integral T>
inline constexpr bool operator()(T ch)
{
	std::make_unsigned_t<T> e(ch);
	if constexpr(sign)
	{
		if constexpr(base<=0xA)
			return (e==0x2d)||static_cast<std::uint8_t>(e-0x30)<base;
		else
		{
			constexpr std::uint8_t basem10(base-0xa);
			return (e==0x2d)||static_cast<std::uint8_t>((e-0x30)<0xA)||
				(static_cast<std::uint8_t>(e-0x41)<basem10)||
				(static_cast<std::uint8_t>(e-0x61)<basem10);
		}
	}
	else
	{
		if constexpr(base<=0xA)
			return static_cast<std::uint8_t>(e-0x30)<base;
		else
		{
			constexpr std::uint8_t basem10(base-0xa);
			return (static_cast<std::uint8_t>(e-0x30)<0xA)||
				(static_cast<std::uint8_t>(e-0x41)<basem10)||
				(static_cast<std::uint8_t>(e-0x61)<basem10);
		}
	}
}
};
}

template<buffer_input_stream input,typename UnaryPredicate>
[[nodiscard]] inline constexpr bool skip_until(input& in,UnaryPredicate pred)
{
	for(;;)
	{
		auto sp(ispan(in));
		for(auto i(sp.data()),e(sp.data()+sp.size());i!=e;++i)
			if(pred(*i))
			{
				icommit(in,i-sp.data());
				return true;
			}
		if(!iflush(in))
			return false;
	}
}

template<buffer_input_stream input>
[[nodiscard]] inline constexpr bool skip_space(input& in)
{
	return skip_until(in,details::is_none_space{});
}

template<bool sign=false,std::uint8_t base=0xA,buffer_input_stream input>
[[nodiscard]] inline constexpr bool skip_none_numerical(input& in)
{
	return skip_until(in,details::is_numerical<sign,base>{});
}



}