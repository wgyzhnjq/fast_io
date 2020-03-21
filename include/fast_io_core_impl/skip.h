#pragma once

namespace fast_io
{
namespace details
{
struct is_none_space
{
template<std::integral T>
inline constexpr bool operator()(T ch) const
{
	std::make_unsigned_t<T> e(ch);
	return 0x4<(e-0x9)&e!=0x20;
}
};

}

template<buffer_input_stream input,typename UnaryPredicate>
[[nodiscard]] inline constexpr bool skip_until(input& in,UnaryPredicate pred)
{
	for(;;)
	{
		decltype(auto) gbegin{ibuffer_gbegin(in)};
		decltype(auto) gend{ibuffer_gend(in)};
		for(;gbegin!=gend;++gbegin)
			if(pred(*gbegin))
				return true;
		if(!underflow(in))
			return false;
	}
}

template<buffer_input_stream input>
[[nodiscard]] inline constexpr bool skip_space(input& in)
{
	return skip_until(in,details::is_none_space{});
}

template<std::size_t sign=false,std::uint8_t base=0xA,buffer_input_stream input>
[[nodiscard]] inline constexpr bool skip_none_numerical(input& in)
{
	return skip_until(in,details::is_numerical<sign,base>{});
}

}