#pragma once

namespace fast_io
{

enum class operating_system
{
	win32,
	posix,
#if defined(__WINNT__) || defined(_MSC_VER)
	native=win32,
#else
	native=posix
#endif
};

template<output_stream T,bool sys=false>
class otext
{
public:
	T ib;
	inline static constexpr bool use_string_as_buffer{!buffer_output_stream<T>&&(operating_system<sys>)};
	std::conditional_t<use_string_as_buffer,std::basic_string<>,void> buffer;
public:
	using native_interface_t = T;
	using char_type = typename native_interface_t::char_type;
public:
	template<typename ...Args>
	requires std::constructible_from<T,Args...>
	constexpr otext(Args&& ...args):ib(std::forward<Args>(args)...){}
	constexpr inline auto& native_handle()
	{
		return ib;
	}
};


}
