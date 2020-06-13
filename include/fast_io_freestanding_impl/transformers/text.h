#pragma once

namespace fast_io
{

namespace transforms
{
struct text_to_binary
{
	template<fast_io::dynamic_buffer_output_stream dyn,std::contiguous_iterator Iter>
	requires std::same_as<typename dyn::char_type,std::iter_value_t<Iter>>
	constexpr Iter operator()(dyn& obuf,Iter begin,Iter end)
	{
		reserve_write(obuf,(end-begin),[&](auto ptr)
		{
			for(;begin!=end;++begin)
			{
				if(*begin==u8'\r')[[unlikely]]
				{
					if(begin+1==end)
						break;
					if(begin[1]==u8'\n')
						++begin;
				}
				*ptr=*begin;
				++ptr;
			}
			return ptr;
		});
		return begin;
	}
};
}

}