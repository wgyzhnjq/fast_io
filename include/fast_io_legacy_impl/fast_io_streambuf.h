#pragma once

namespace fast_io
{

template<stream stm,typename traits_tp = std::char_traits<typename stm::char_type>>
class fast_io_basic_streambuf final:public std::basic_streambuf<typename traits_tp::char_type,traits_tp>
{
public:
	using native_handle_type = stm;
	using traits_type = traits_tp;
	using char_type = typename traits_type::char_type;
private:
	native_handle_type sm;
	std::streamsize xsputn(char_type const* s, std::streamsize count)
#ifndef _MSC_VER
requires(output_stream<native_handle_type>)
#endif
override
	{
#ifdef _MSC_VER
		if constexpr(output_stream<native_handle_type>)
		{
#endif
		if constexpr(!std::same_as<decltype(write(sm,s,s+count)),void>)
			return static_cast<std::streamsize>(write(sm,s,s+count)-s);
		else
		{
			write(sm,s,s+count);
			return count;
		}
#ifdef _MSC_VER
		}
#endif
	}
	std::streamsize xsgetn(char_type* s, std::streamsize count)
#ifndef _MSC_VER
requires(input_stream<native_handle_type>)
#endif
override
	{
#ifdef _MSC_VER
		if constexpr(input_stream<native_handle_type>)
		{
#endif
		return static_cast<std::streamsize>(read(sm,s,s+count)-s);
#ifdef _MSC_VER
		}
#endif
	}
public:
	template<typename... Args>
	requires std::constructible_from<stm,Args...>
	fast_io_basic_streambuf(Args&& ...args):sm(std::forward<Args>(args)...){}
	constexpr auto& native_handle()
	{
		return sm;
	}
};

}