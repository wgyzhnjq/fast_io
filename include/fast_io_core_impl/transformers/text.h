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
	inline constexpr void operator()(output& out,Iter begin,Iter end)
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
	}
};

template<buffer_output_stream T,bool sys=false>
using obinary_to_text=otransform_function_default_construct<T,binary_to_text<sys>>;

}
