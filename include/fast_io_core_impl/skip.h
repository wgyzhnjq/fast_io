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

}

template<buffer_input_stream input,typename UnaryPredicate>
[[nodiscard]] inline constexpr bool skip_until(input& in,UnaryPredicate pred)
{
	for(;;)
	{
		for(auto i(begin(in)),e(end(in));i!=e;++i)
			if(pred(*i))
			{
				in+=i-begin(in);
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

template<bool report_eof=false,buffer_input_stream bip>
[[nodiscard]] inline constexpr auto front(bip& input)->std::conditional_t<report_eof,std::pair<typename bip::char_type,bool>,typename bip::char_type>
{
	if(begin(input)==end(input))
	{
		if(!iflush(input))
		{
			if constexpr(report_eof)
				return {0,false};
			else
			{
#ifdef __cpp_exceptions
				throw eof();
#else
				std::terminate();
#endif
			}
		}
	}
	if constexpr(report_eof)
		return {*begin(input),true};
	else
		return *begin(input);
}

template<bool report_eof=false,buffer_input_stream bip>
[[nodiscard]] inline constexpr auto get(bip& input)->std::conditional_t<report_eof,std::pair<typename bip::char_type,bool>,typename bip::char_type>
{
	if(begin(input)==end(input))
	{
		if(!iflush(input))
		{
			if constexpr(report_eof)
				return {0,false};
			else
			{
#ifdef __cpp_exceptions
				throw eof();
#else
				std::terminate();
#endif
			}
		}
	}
	++input;
	if constexpr(report_eof)
		return {*begin(input),true};
	else
		return *begin(input);
}
}