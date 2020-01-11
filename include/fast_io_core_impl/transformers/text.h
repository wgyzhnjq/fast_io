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

template<bool sys=false>
class binary_to_text
{
public:
template<buffer_output_stream output,std::contiguous_iterator Iter>
inline constexpr Iter write_proxy(output& out,Iter begin,Iter end)
{
	if constexpr(!sys||operating_system::native==operating_system::win32)
		for(auto iter(begin);iter!=end;++iter)
			if(*iter==u8'\n')
			{
				write(out,begin,iter);
				put(out,u8'\r');
				begin=iter;
			}
	write(out,begin,end);
	return end;
}
};
/*
template<bool sys=false>
class text_to_binary
{
	std::conditional_t<!sys||operating_system::native==operating_system::win32,bool,void> last_is_carriage{};
public:
template<buffer_output_stream output,std::contiguous_iterator Iter>
inline constexpr void operator()(output& out,Iter begin,Iter end)
{
	if constexpr(!sys||operating_system::native==operating_system::win32)
	{
		if(begin==end)
			return;
		if(last_is_carriage&&*begin!=u8'\n')
			put(out,'\r');
		last_is_carriage={};
		for(auto iter(begin);iter!=end;++iter)
			if(*iter==u8'\r')
			{
				write(out,begin,iter);
				put(out,u8'\r');
				begin=iter;
			}

	}
	else
		write(out,begin,end);
}
};
*/
template<buffer_output_stream T,std::integral ch_type=typename T::char_type,std::size_t sz=4096>
using obinary_to_text=otransform_function_default_construct<T,binary_to_text<false>,ch_type,sz>;
template<buffer_output_stream T,std::integral ch_type=typename T::char_type,std::size_t sz=4096>
using obinary_to_native_text=otransform_function_default_construct<T,binary_to_text<true>,ch_type,sz>;

}
