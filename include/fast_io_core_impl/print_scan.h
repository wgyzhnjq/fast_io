#pragma once

namespace fast_io
{


namespace details
{
template<character_input_stream input,typename T>
requires (general_scanable<input,T>)
inline constexpr auto scan_with_space(input &in,T&& t)
{
	if constexpr(space_scanable<input,T>)
	{
		if(!skip_space(in))
			return false;
		space_scan_define(in,std::forward<T>(t));
		return true;
	}
	else
		return scan_define(in,std::forward<T>(t));
}

template<character_input_stream input,typename T>
inline constexpr void scan_with_ex(input &in,T&& t)
{
	if constexpr(std::same_as<decltype(scan_with_space(in,std::forward<T>(t))),void>)
		scan_with_space(in,std::forward<T>(t));
	else
	{
		if(!scan_with_space(in,t))
			FIO_EOF();
	}
}

template<bool report_eof,character_input_stream input,typename ...Args>
requires(general_scanable<input,Args>&&...)
inline constexpr auto normal_scan(input &ip,Args&& ...args)
{
	if constexpr(report_eof)
		return (static_cast<std::size_t>(scan_with_space(ip,std::forward<Args>(args)))+...);
	else
		(scan_with_ex(ip,std::forward<Args>(args)),...);
}


template<output_stream output,typename T>
requires (printable<output,T>||reserve_printable<T>)
inline constexpr void print_control(output& out,T&& t)
{
	using char_type = typename output::char_type;
	using no_cvref = std::remove_cvref_t<T>;
	if constexpr(reserve_printable<T>)
	{
		constexpr std::size_t size{print_reserve_size(print_reserve_type<no_cvref>)};
		if constexpr(reserve_output_stream<output>)
		{
			if constexpr(std::is_pointer_v<std::remove_cvref_t<decltype(oreserve(out,size))>>)
			{
				auto ptr{oreserve(out,size)};
				if(ptr==nullptr)[[unlikely]]
				{
					std::array<char_type,size> array;
					write(out,array.data(),print_reserve_define(print_reserve_type<no_cvref>,array.data(),std::forward<T>(t)));
					return;
				}
				orelease(out,print_reserve_define(print_reserve_type<no_cvref>,ptr,std::forward<T>(t)));
			}
			else
			{
				orelease(out,print_reserve_define(print_reserve_type<no_cvref>,oreserve(out,size),std::forward<T>(t)));
			}
		}
		else
		{
			std::array<char_type,size> array;
			write(out,array.data(),print_reserve_define(print_reserve_type<no_cvref>,array.data(),std::forward<T>(t)));
		}
	}
	else if constexpr(printable<output,T>)
	{
		print_define(out,std::forward<T>(t));
	}
}

template<output_stream output,typename T,std::integral ch_type>
requires ((printable<output,T>&&character_output_stream<output>)||reserve_printable<T>)
inline constexpr void print_control(output& out,manip::follow_character<T,ch_type> t)
{
	if constexpr(reserve_printable<T>)
	{
		using char_type = typename output::char_type;
		constexpr std::size_t size{print_reserve_size(print_reserve_type<std::remove_cvref_t<T>>)+1};
		if constexpr(reserve_output_stream<output>)
		{
			if constexpr(std::is_pointer_v<std::remove_cvref_t<decltype(oreserve(out,size))>>)
			{
				auto ptr{oreserve(out,size)};
				if(ptr==nullptr)[[unlikely]]
				{
					std::array<char_type,size> array;
					auto it{print_reserve_define(print_reserve_type<std::remove_cvref_t<T>>,array.data(),t.reference)};
					*it=t.character;
					write(out,array.data(),++it);
					return;
				}
				auto it{print_reserve_define(print_reserve_type<std::remove_cvref_t<T>>,ptr,t.reference)};
				*it=t.character;
				orelease(out,++it);
			}
			else
			{
				auto it{print_reserve_define(print_reserve_type<std::remove_cvref_t<T>>,oreserve(out,size),t.reference)};
				*it=t.character;
				orelease(out,++it);
			}
		}
		else
		{
			std::array<char_type,size> array;
			auto it{print_reserve_define(print_reserve_type<std::remove_cvref_t<T>>,array.data(),t.reference)};
			*it=t.character;
			write(out,array.data(),++it);
		}
	}
	else if constexpr(printable<output,T>)
	{
		print_define(out,t.reference);
		put(out,t.character);
	}
}

template<output_stream output,typename T>
requires ((printable<output,T>&&character_output_stream<output>)||reserve_printable<T>)
inline constexpr void print_control_line(output& out,T&& t)
{
	if constexpr(reserve_printable<T>)
	{
		using char_type = typename output::char_type;
		constexpr std::size_t size{print_reserve_size(print_reserve_type<std::remove_cvref_t<T>>)+1};
		if constexpr(reserve_output_stream<output>)
		{
			if constexpr(std::is_pointer_v<std::remove_cvref_t<decltype(oreserve(out,size))>>)
			{
				auto ptr{oreserve(out,size)};
				if(ptr==nullptr)[[unlikely]]
				{
					std::array<char_type,size> array;
					auto it{print_reserve_define(print_reserve_type<std::remove_cvref_t<T>>,array.data(),std::forward<T>(t))};
					*it=u8'\n';
					write(out,array.data(),++it);
					return;
				}
				auto it{print_reserve_define(print_reserve_type<std::remove_cvref_t<T>>,ptr,std::forward<T>(t))};
				*it=u8'\n';
				orelease(out,++it);
			}
			else
			{
				auto it{print_reserve_define(print_reserve_type<std::remove_cvref_t<T>>,oreserve(out,size),std::forward<T>(t))};
				*it=u8'\n';
				orelease(out,++it);
			}
		}
		else
		{
			std::array<char_type,size> array;
			auto it{print_reserve_define(print_reserve_type<std::remove_cvref_t<T>>,array.data(),std::forward<T>(t))};
			*it=u8'\n';
			write(out,array.data(),++it);
		}
	}
	else if constexpr(printable<output,T>)
	{
		print_define(out,std::forward<T>(t));
		put(out,u8'\n');
	}
}

/*
template<output_stream output,typename ...Args>
requires(general_printable<output,Args>&&...)
inline constexpr void normal_print(output &out,Args&& ...args)
{
	(print_control(out,std::forward<Args>(args)),...);
}

template<output_stream output,typename ...Args>
requires((sizeof...(Args)==1&&(reserve_printable<Args>&&...))||(character_output_stream<output>&&(printable<output,Args>&&...)))
inline constexpr void normal_println(output &out,Args&& ...args)
{
	if constexpr((sizeof...(Args)==1)&&(reserve_printable<Args>&&...))
	{
		((print_control_line(out,std::forward<Args>(args))),...);
	}
	else
	{
		((print_control(out,std::forward<Args>(args))),...);
		put(out,u8'\n');
	}
}
*/
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

template<output_stream output,typename T>
requires ((printable<output,T>&&character_output_stream<output>)||reserve_printable<T>)
inline constexpr void print_define(output& out,manip::line<T> t)
{
	details::print_control_line(out,t.reference);
}

template<reserve_printable T>
inline constexpr std::size_t print_reserve_size(print_reserve_type_t<manip::line<T>>)
{
	constexpr std::size_t sz{print_reserve_size(print_reserve_type<std::remove_cvref_t<T>>)+1};
	return sz;
}

template<std::random_access_iterator raiter,reserve_printable T,typename U>
inline constexpr raiter print_reserve_define(print_reserve_type_t<manip::line<T>>,raiter start,U a)
{
	auto it{print_reserve_define(print_reserve_type<std::remove_cvref_t<T>>,start,a.reference)};
	*it=u8'\n';
	return ++it;
}

template<bool report_eof=false,input_stream input,typename ...Args>
inline constexpr auto scan(input &&in,Args&& ...args)
{
	if constexpr(mutex_input_stream<input>)
	{
		typename std::remove_cvref_t<input>::lock_guard_type lg{mutex(in)};
		decltype(auto) uh(unlocked_handle(in));
		return scan<report_eof>(uh,std::forward<Args>(args)...);
	}
	else if constexpr(status_input_stream<input>)
		return scan_status_define<report_eof>(in,std::forward<Args>(args)...);
	else
		return details::normal_scan<report_eof>(in,std::forward<Args>(args)...);
}

namespace details
{


template<std::integral char_type,typename T>
inline constexpr void scatter_print_recursive(io_scatter_t* arr,T&& t)
{
	*arr=print_scatter_define<char_type>(std::forward<T>(t));
}

template<std::integral char_type,typename T,typename... Args>
inline constexpr void scatter_print_recursive(io_scatter_t* arr,T&& t, Args&& ...args)
{
	*arr=print_scatter_define<char_type>(std::forward<T>(t));
	scatter_print_recursive<char_type>(arr+1,std::forward<Args>(args)...);
}

template<std::integral char_type,typename T>
inline constexpr void scatter_print_with_buffer_recursive_unit(internal_temporary_buffer<char_type>& b,
		io_scatter_t* arr,T&& t)
{
	if constexpr(scatter_printable<char_type,T>)
	{
		*arr=print_scatter_define<char_type>(std::forward<T>(t));
	}
	else
	{
		auto start_ptr{b.end_ptr};
		print_control(b,std::forward<T>(t));
		*arr={start_ptr,(b.end_ptr-start_ptr)*sizeof(*start_ptr)};
	}
}

template<std::integral char_type,typename T>
inline constexpr void scatter_print_with_buffer_recursive(internal_temporary_buffer<char_type>& b,
		io_scatter_t* arr,T&& t)
{
	scatter_print_with_buffer_recursive_unit(b,arr,std::forward<T>(t));
}

template<std::integral char_type,typename T,typename... Args>
inline constexpr void scatter_print_with_buffer_recursive(internal_temporary_buffer<char_type>& b,
	io_scatter_t* arr,T&& t, Args&& ...args)
{
	scatter_print_with_buffer_recursive_unit(b,arr,std::forward<T>(t));
	scatter_print_with_buffer_recursive(b,arr+1,std::forward<Args>(args)...);
}


template<typename T>
inline constexpr std::size_t calculate_scatter_reserve_size_unit()
{
	using real_type = std::remove_cvref_t<T>;
	if constexpr(reserve_printable<real_type>)
	{
		constexpr std::size_t sz{print_reserve_size(print_reserve_type<real_type>)};
		return sz;
	}
	else
		return 0;
}

template<typename T>
inline constexpr std::size_t calculate_scatter_reserve_size()
{
	return calculate_scatter_reserve_size_unit<T>();
}

template<typename T,typename... Args>
requires (sizeof...(Args)!=0)
inline constexpr std::size_t calculate_scatter_reserve_size()
{
	return calculate_scatter_reserve_size_unit<T>()+
		calculate_scatter_reserve_size<Args...>();
}

template<std::integral char_type,typename T>
inline constexpr void scatter_print_with_reserve_recursive_unit(char_type*& start_ptr,
		io_scatter_t* arr,T&& t)
{
	if constexpr(scatter_printable<char_type,T>)
	{
		*arr=print_scatter_define<char_type>(std::forward<T>(t));
	}
	else
	{
		using real_type = std::remove_cvref_t<T>;
		auto end_ptr = print_reserve_define(print_reserve_type<real_type>,start_ptr,std::forward<T>(t));
		*arr={start_ptr,(end_ptr-start_ptr)*sizeof(*start_ptr)};
		start_ptr=end_ptr;
	}
}



template<std::integral char_type,typename T>
inline constexpr void scatter_print_with_reserve_recursive(char_type* ptr,
		io_scatter_t* arr,T&& t)
{
	scatter_print_with_reserve_recursive_unit(ptr,arr,std::forward<T>(t));
}

template<std::integral char_type,typename T,typename... Args>
inline constexpr void scatter_print_with_reserve_recursive(char_type* ptr,
	io_scatter_t* arr,T&& t, Args&& ...args)
{
	scatter_print_with_reserve_recursive_unit(ptr,arr,std::forward<T>(t));
	scatter_print_with_reserve_recursive(ptr,arr+1,std::forward<Args>(args)...);
}

template<bool line,output_stream output,typename ...Args>
inline constexpr void print_fallback(output &out,Args&& ...args)
{
	if constexpr(scatter_output_stream<output>&&((scatter_printable<typename output::char_type,Args>||reserve_printable<Args>)&&...))
	{
		std::array<io_scatter_t,(sizeof...(Args))+static_cast<std::size_t>(line)> scatters;
		if constexpr((scatter_printable<typename output::char_type,Args>&&...))
		{
			scatter_print_recursive<typename output::char_type>(scatters.data(),std::forward<Args>(args)...);
			if constexpr(line)
			{
				typename output::char_type ch(u8'\n');
				scatters.back()={std::addressof(ch),sizeof(ch)};
				scatter_write(out,scatters);
			}
			else
				scatter_write(out,scatters);
		}
		else
		{
			std::array<typename output::char_type,calculate_scatter_reserve_size<Args...>()+static_cast<std::size_t>(line)> array;
			scatter_print_with_reserve_recursive(array.data(),scatters.data(),std::forward<Args>(args)...);
			if constexpr(line)
			{
				typename output::char_type ch(u8'\n');
				scatters.back()={std::addressof(ch),sizeof(ch)};
				scatter_write(out,scatters);
			}
			else
				scatter_write(out,scatters);
		}
	}
	else
	{
		internal_temporary_buffer<typename output::char_type> buffer;
		if constexpr(line)
		{
			if constexpr((sizeof...(Args)==1)&&(reserve_printable<Args>&&...))
			{
				((details::print_control_line(buffer,std::forward<Args>(args))),...);
			}
			else
			{
				((details::print_control(buffer,std::forward<Args>(args))),...);
				put(buffer,u8'\n');
			}
		}
		else
		{
			(details::print_control(buffer,std::forward<Args>(args)),...);
		}
		write(out,buffer.beg_ptr,buffer.end_ptr);
	}
}
}

template<output_stream output,typename callback>
inline constexpr void print_transaction(output &&out,callback func)
{
	internal_temporary_buffer<typename output::char_type> buffer;
	func(buffer);
	write(out,buffer.beg_ptr,buffer.end_ptr);
}

template<output_stream output,typename ...Args>
inline constexpr void print(output &&out,Args&& ...args)
{
	if constexpr(mutex_output_stream<output>)
	{
		typename std::remove_cvref_t<output>::lock_guard_type lg{mutex(out)};
		decltype(auto) uh(unlocked_handle(out));
		print(uh,std::forward<Args>(args)...);
	}
	else if constexpr(status_output_stream<output>)
		print_status_define(out,std::forward<Args>(args)...);
	else if constexpr(((printable<output,Args>||reserve_printable<Args>)&&...)&&(sizeof...(Args)==1||buffer_output_stream<output>))
	{
		if constexpr(sizeof...(Args)==1||(!maybe_buffer_output_stream<output>))
			(details::print_control(out,std::forward<Args>(args)),...);
		else
		{
			if(!obuffer_is_active(out))[[unlikely]]
			{
				details::print_fallback<false>(out,std::forward<Args>(args)...);
				return;
			}
			(details::print_control(out,std::forward<Args>(args)),...);
		}
	}
	else
		details::print_fallback<false>(out,std::forward<Args>(args)...);
}

template<output_stream output,typename ...Args>
inline constexpr void println(output &&out,Args&& ...args)
{
	if constexpr(mutex_output_stream<output>)
	{
		typename std::remove_cvref_t<output>::lock_guard_type lg{mutex(out)};
		decltype(auto) uh(unlocked_handle(out));
		println(uh,std::forward<Args>(args)...);
	}
	else if constexpr(status_output_stream<output>)
		println_status_define(out,std::forward<Args>(args)...);
	else if constexpr((sizeof...(Args)==1&&(reserve_printable<Args>&&...))||
	((printable<output,Args>&&...)&&buffer_output_stream<output>&&character_output_stream<output>))
	{
		if constexpr((sizeof...(Args)==1)&&(reserve_printable<Args>&&...))
			((details::print_control_line(out,std::forward<Args>(args))),...);
		else
		{
			if constexpr(maybe_buffer_output_stream<output>)
			{
				if(!obuffer_is_active(out))[[unlikely]]
				{
					details::print_fallback<true>(out,std::forward<Args>(args)...);
					return;
				}
			}
			((details::print_control(out,std::forward<Args>(args))),...);
			put(out,u8'\n');
		}
	}
	else
		details::print_fallback<true>(out,std::forward<Args>(args)...);
}
/*
template<output_stream output,typename ...Args>
requires (sizeof...(Args)!=0)
inline constexpr void send(output &out,Args&& ...args)
{
	if constexpr(mutex_output_stream<output>)
	{
		typename output::lock_guard_type lg{mutex(out)};
		decltype(auto) uh(unlocked_handle(out));
		send(uh,std::forward<Args>(args)...);
	}
	else if constexpr((sendable<output,Args>&&...)&&(sizeof...(Args)==1||buffer_output_stream<output>))
	{
		(send_define(out,std::forward<Args>(args)),...);
	}
	else
	{
		internal_temporary_buffer<typename output::char_type> buffer;
		(send_define(buffer,std::forward<Args>(args)),...);
		write(out,buffer.beg_ptr,buffer.end_ptr);
	}
}
*/
#ifndef NDEBUG

template<typename ...Args>
inline constexpr void debug_print(Args&& ...args)
{
	fast_io::print(std::forward<Args>(args)...);
}

template<typename ...Args>
inline constexpr void debug_println(Args&& ...args)
{
	fast_io::debug_println(std::forward<Args>(args)...);
}

#endif
/*
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

*/

}
