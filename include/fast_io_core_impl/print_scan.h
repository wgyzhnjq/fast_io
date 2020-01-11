#pragma once

namespace fast_io
{



template<buffer_input_stream input>
inline constexpr std::size_t skip_line(input& in)
{
	std::size_t skipped(0);
	for(decltype(get<true>(in)) ch;!(ch=get<true>(in)).second&&ch.first!=0xA;++skipped);
	return skipped;
}
/*

template<buffer_input_stream input>
inline constexpr auto eat_space_get(input& in)
{
	decltype(get(in)) ch(get(in));
	for(;details::isspace(ch);ch=get(in));
	return ch;
}

template<buffer_input_stream input>
inline constexpr auto try_eat_space_get(input& in)
{
	auto ch(get<true>(in));
	for(;details::isspace(ch.first);ch=get<true>(in));
	return ch;
}

template<buffer_input_stream input,std::integral T>
requires std::same_as<T,bool>
inline constexpr void scan_define(input& in, T& b)
{
	auto value(eat_space_get(in));
	if(value==0x30)
		b=0;
	else
		b=1;
}
*/
template<character_output_stream output,std::integral T>
requires std::same_as<T,bool>
inline constexpr void print_define(output& out, T const& b)
{
	put(out,b+0x30);
}

template<output_stream output>
inline constexpr void print_define(output& out,std::basic_string_view<typename output::char_type> str)
{
	write(out,str.data(),str.data()+str.size());
}

inline namespace print_scan_details
{

template<buffer_input_stream input,typename T>
inline constexpr void scan_with_ex(input &in,T&& t)
{
	if constexpr(std::same_as<decltype(scan_define(in,std::forward<T>(t))),void>)
		scan_define(in,std::forward<T>(t));
	else
	{
		if(!scan_define(in,t))
#ifdef __cpp_exceptions
			throw eof();
#else
			fast_terminate();
#endif
	}
}

template<bool report_eof,buffer_input_stream input,typename ...Args>
requires(scanable<input,Args>&&...)
inline constexpr auto normal_scan(input &ip,Args&& ...args)
{
	if constexpr(report_eof)
		return (static_cast<std::size_t>(scan_define(ip,std::forward<Args>(args)))+...);
	else
		(scan_with_ex(ip,std::forward<Args>(args)),...);
}

template<output_stream output,typename ...Args>
requires(printable<output,Args>&&...)
inline constexpr void normal_print(output &out,Args&& ...args)
{
	(print_define(out,std::forward<Args>(args)),...);
}

template<output_stream output,typename ...Args>
requires((sizeof...(Args)==1&&(printlnable<output,Args>&&...))||(character_output_stream<output>&&(printable<output,Args>&&...)))
inline constexpr void normal_println(output &out,Args&& ...args)
{
	if constexpr((sizeof...(Args)==1)&&(printlnable<output,Args>&&...))
	{
		(println_define(out,std::forward<Args>(args)),...);
	}
	else
	{
		(print_define(out,std::forward<Args>(args)),...);
		put(out,0xA);
	}
}

template<input_stream input,typename ...Args>
requires(receiveable<input,Args>&&...)
inline constexpr void normal_receive(input &in,Args&& ...args)
{
	(read_define(in,std::forward<Args>(args)),...);
}

template<output_stream output,typename ...Args>
requires(sendable<output,Args>&&...)
inline constexpr void normal_send(output &out,Args&& ...args)
{
	(send_define(out,std::forward<Args>(args)),...);
}

}

template<bool report_eof=false,input_stream input,typename ...Args>
requires (sizeof...(Args)!=0)
inline constexpr auto scan(input &in,Args&& ...args)
{
	using namespace print_scan_details;
	if constexpr(mutex_input_stream<input>)
	{
		typename input::lock_guard_type lg{mutex(in)};
		decltype(auto) uh(unlocked_handle(in));
		return scan<report_eof>(uh,std::forward<Args>(args)...);
	}
	else
		return normal_scan<report_eof>(in,std::forward<Args>(args)...);
}

template<input_stream input,typename ...Args>
requires (sizeof...(Args)!=0)
inline constexpr void receive(input &in,Args&& ...args)
{
	using namespace print_scan_details;
	if constexpr(mutex_input_stream<input>)
	{
		typename input::lock_guard_type lg{mutex(in)};
		decltype(auto) uh(unlocked_handle(in));
		receive(uh,std::forward<Args>(args)...);
	}
	else if constexpr(true)
		normal_receive(in,std::forward<Args>(args)...);
}

template<output_stream output,typename ...Args>
requires (sizeof...(Args)!=0)
inline constexpr void print(output &out,Args&& ...args)
{
	using namespace print_scan_details;
	if constexpr(mutex_output_stream<output>)
	{
		typename output::lock_guard_type lg{mutex(out)};
		decltype(auto) uh(unlocked_handle(out));
		print(uh,std::forward<Args>(args)...);
	}
	else if constexpr((printable<output,Args>&&...)&&(sizeof...(Args)==1||buffer_output_stream<output>))
		normal_print(out,std::forward<Args>(args)...);
	else if constexpr(true)
		buffer_print(out,std::forward<Args>(args)...);
}

template<output_stream output,typename ...Args>
inline constexpr void println(output &out,Args&& ...args)
{
	using namespace print_scan_details;
	if constexpr(mutex_output_stream<output>)
	{
		typename output::lock_guard_type lg{mutex(out)};
		decltype(auto) uh(unlocked_handle(out));
		println(uh,std::forward<Args>(args)...);
	}
	else if constexpr((sizeof...(Args)==1&&(printlnable<output,Args>&&...))||
	((printable<output,Args>&&...)&&buffer_output_stream<output>&&character_output_stream<output>))
		normal_println(out,std::forward<Args>(args)...);
	else if constexpr(true)
		buffer_println(out,std::forward<Args>(args)...);
}

template<output_stream output,typename ...Args>
requires (sizeof...(Args)!=0)
inline constexpr void send(output &out,Args&& ...args)
{
	using namespace print_scan_details;
	if constexpr(mutex_output_stream<output>)
	{
		typename output::lock_guard_type lg{mutex(out)};
		decltype(auto) uh(unlocked_handle(out));
		send(uh,std::forward<Args>(args)...);
	}
	else if constexpr((sendable<output,Args>&&...)&&(sizeof...(Args)==1||buffer_output_stream<output>))
		normal_send(out,std::forward<Args>(args)...);
	else if constexpr(true)
		buffer_send(out,std::forward<Args>(args)...);
}


template<output_stream output,typename ...Args>
requires (sizeof...(Args)!=0)
inline constexpr void print_flush(output &out,Args&& ...args)
{
	using namespace print_scan_details;
	if constexpr(mutex_output_stream<output>)
	{
		typename output::lock_guard_type lg{mutex(out)};
		decltype(auto) uh(unlocked_handle(out));
		print_flush(uh,std::forward<Args>(args)...);
	}
	else
	{
		if constexpr((printable<output,Args>&&...)&&(sizeof...(Args)==1||buffer_output_stream<output>))
			normal_print(out,std::forward<Args>(args)...);
		else if constexpr(true)
			buffer_print(out,std::forward<Args>(args)...);
		flush(out);
	}
}

template<output_stream output,typename ...Args>
inline constexpr void println_flush(output &out,Args&& ...args)
{
	using namespace print_scan_details;
	if constexpr(mutex_output_stream<output>)
	{
		typename output::lock_guard_type lg{mutex(out)};
		decltype(auto) uh(unlocked_handle(out));
		println_flush(out.native_handle(),std::forward<Args>(args)...);
	}
	else
	{
		if constexpr((sizeof...(Args)==1&&(printlnable<output,Args>&&...))||
			((printable<output,Args>&&...)&&buffer_output_stream<output>&&character_output_stream<output>))
			normal_println(out,std::forward<Args>(args)...);
		else if constexpr(true)
			buffer_println(out,std::forward<Args>(args)...);
		flush(out);
	}
}

template<output_stream output,typename ...Args>
requires (sizeof...(Args)!=0)
inline constexpr void send_flush(output &out,Args&& ...args)
{
	using namespace print_scan_details;
	if constexpr(mutex_output_stream<output>)
	{
		typename output::lock_guard_type lg{mutex(out)};
		decltype(auto) uh(unlocked_handle(out));
		send_flush(out.native_handle(),std::forward<Args>(args)...);
	}
	else
	{
		if constexpr((sendable<output,Args>&&...)&&(sizeof...(Args)==1||buffer_output_stream<output>))
			normal_send(out,std::forward<Args>(args)...);
		else if constexpr(true)
			buffer_send(out,std::forward<Args>(args)...);
		flush(out);
	}
}


inline namespace print_scan_details
{
template<output_stream os,typename ch_type,typename ...Args>
inline void fprint_impl(os &out,std::basic_string_view<ch_type> format)
{
	std::size_t percent_pos;
	for(;(percent_pos=format.find(0x25))!=std::string_view::npos&&percent_pos+1!=format.size()&&format[percent_pos+1]==0X25;format.remove_prefix(percent_pos+2))
		write(out,format.cbegin(),format.cbegin()+percent_pos+1);
	if(percent_pos!=std::string_view::npos)
		fast_terminate();
	write(out,format.cbegin(),format.cend());
}

template<output_stream os,typename ch_type,typename T,typename ...Args>
inline void fprint_impl(os &out,std::basic_string_view<ch_type> format,T&& cr,Args&& ...args)
{
	std::size_t percent_pos;
	for(;(percent_pos=format.find(0x25))!=std::string_view::npos&&percent_pos+1!=format.size()&&format[percent_pos+1]==0x25;format.remove_prefix(percent_pos+2))
		write(out,format.cbegin(),format.cbegin()+percent_pos+1);
	if(percent_pos==std::string_view::npos)
	{
		write(out,format.cbegin(),format.cend());
		return;
	}
	else
	{
		write(out,format.cbegin(),format.cbegin()+percent_pos);
		format.remove_prefix(percent_pos+1);
	}
	print(out,std::forward<T>(cr));
	fprint_impl(out,format,std::forward<Args>(args)...);
}

template<output_stream output,typename ...Args>
requires(printable<output,Args>&&...)
inline constexpr void normal_fprint(output &out,std::basic_string_view<typename output::char_type> mv,Args&& ...args)
{
	fprint_impl(out,mv,std::forward<Args>(args)...);
}

template<output_stream output,typename ...Args>
requires(std::same_as<typename output::char_type,char>&&(printable<output,Args>&&...))
inline constexpr void normal_fprint(output &out,std::basic_string_view<char8_t> mv,Args&& ...args)
{
	fprint_impl(out,mv,std::forward<Args>(args)...);
}

}

template<output_stream output,typename ...Args>
requires (sizeof...(Args)!=0)
inline constexpr void fprint(output &out,Args&& ...args)
{
	using namespace print_scan_details;
	if constexpr(mutex_output_stream<output>)
	{
		typename output::lock_guard_type lg{mutex(out)};
		decltype(auto) uh(unlocked_handle(out));
		fprint(uh,std::forward<Args>(args)...);
	}
	else if constexpr((printable<output,Args>&&...)&&(sizeof...(Args)==1||(buffer_output_stream<output>&&character_output_stream<output>)))
		normal_fprint(out,std::forward<Args>(args)...);
	else if constexpr(true)
		buffer_fprint(out,std::forward<Args>(args)...);
}

template<output_stream output,typename ...Args>
requires (sizeof...(Args)!=0)
inline constexpr void fprint_flush(output &out,Args&& ...args)
{
	using namespace print_scan_details;
	if constexpr(mutex_output_stream<output>)
	{
		typename output::lock_guard_type lg{mutex(out)};
		decltype(auto) uh(unlocked_handle(out));
		fprint_flush(uh,std::forward<Args>(args)...);
	}
	else
	{
		if constexpr((printable<output,Args>&&...)&&(sizeof...(Args)==1||buffer_output_stream<output>))
			normal_fprint(out,std::forward<Args>(args)...);
		else if constexpr(true)
			buffer_fprint(out,std::forward<Args>(args)...);
		flush(out);
	}
}


}