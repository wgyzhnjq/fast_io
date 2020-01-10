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
	return 0x4<(e-0x9)&e!=0x20;
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

template<std::size_t sign=false,std::uint8_t base=0xA,buffer_input_stream input>
[[nodiscard]] inline constexpr bool skip_none_numerical(input& in)
{
	return skip_until(in,details::is_numerical<sign,base>{});
}

template<std::size_t report_eof=0,buffer_input_stream bip>
[[nodiscard]] inline constexpr auto front(bip& input)->std::conditional_t<report_eof==1,std::pair<typename bip::char_type,bool>,typename bip::char_type>
{
	if(begin(input)==end(input))
	{
		if(!iflush(input))
		{
			if constexpr(report_eof==1)
				return {0,false};
			else if constexpr(report_eof==2)
				return 0;
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
	if constexpr(report_eof==1)
		return {*begin(input),true};
	else
		return *begin(input);
}

template<std::size_t report_eof=0,buffer_input_stream bip>
[[nodiscard]] inline constexpr auto get(bip& input)->std::conditional_t<report_eof==1,std::pair<typename bip::char_type,bool>,typename bip::char_type>
{
	if(begin(input)==end(input))
	{
		if(!iflush(input))
		{
			if constexpr(report_eof==1)
				return {0,false};
			else if constexpr(report_eof==2)
				return 0;
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
	if constexpr(report_eof==1)
		return {*begin(input),true};
	else
		return *begin(input);
}

template<std::size_t report_eof=0,buffer_input_stream bip>
[[nodiscard]] inline constexpr auto next(bip& input)->std::conditional_t<report_eof==1,std::pair<typename bip::char_type,bool>,typename bip::char_type>
{
	if(begin(input)==end(input))
	{
		if(!iflush(input))
		{
			if constexpr(report_eof==1)
				return {0,false};
			else if constexpr(report_eof==2)
				return 0;
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
	else [[likely]]
		++input;
	if constexpr(report_eof==1)
		return {*begin(input),true};
	else
		return *begin(input);
}

template<std::size_t report_eof=0,buffer_input_stream bip>
[[nodiscard]] inline constexpr auto front_unsigned(bip& input)
	->std::conditional_t<report_eof==1,std::pair<std::make_unsigned_t<typename bip::char_type>,bool>,std::make_unsigned_t<typename bip::char_type>>
{
	using unsigned_char_type = std::make_unsigned_t<typename bip::char_type>;
	if(begin(input)==end(input))
	{
		if(!iflush(input))
		{
			if constexpr(report_eof==1)
				return {0x0,false};
			else if constexpr(report_eof==2)
				return 0x0;
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
	if constexpr(report_eof==1)
		return {static_cast<unsigned_char_type>(*begin(input)),true};
	else
		return static_cast<unsigned_char_type>(*begin(input));
}

template<std::size_t report_eof=0,buffer_input_stream bip>
[[nodiscard]] inline constexpr auto get_unsigned(bip& input)
	->std::conditional_t<report_eof==1,std::pair<std::make_unsigned_t<typename bip::char_type>,bool>,std::make_unsigned_t<typename bip::char_type>>
{
	using unsigned_char_type = std::make_unsigned_t<typename bip::char_type>;
	if(begin(input)==end(input))
	{
		if(!iflush(input))
		{
			if constexpr(report_eof==1)
				return {0x0,false};
			else if constexpr(report_eof==2)
				return 0x0;
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
	if constexpr(report_eof==1)
		return {static_cast<unsigned_char_type>(*begin(input)),true};
	else
		return static_cast<unsigned_char_type>(*begin(input));
}

template<std::size_t report_eof=0,buffer_input_stream bip>
[[nodiscard]] inline constexpr auto next_unsigned(bip& input)
	->std::conditional_t<report_eof==1,std::pair<std::make_unsigned_t<typename bip::char_type>,bool>,std::make_unsigned_t<typename bip::char_type>>
{
	using unsigned_char_type = std::make_unsigned_t<typename bip::char_type>;
	if(begin(++input)==end(input))
	{
		if(!iflush(input))
		{
			if constexpr(report_eof==1)
				return {0x0,false};
			else if constexpr(report_eof==2)
				return 0x0;
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
	if constexpr(report_eof==1)
		return {static_cast<unsigned_char_type>(*begin(input)),true};
	else
		return static_cast<unsigned_char_type>(*begin(input));
}
}