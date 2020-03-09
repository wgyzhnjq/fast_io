#pragma once

namespace fast_io
{


template<buffer_input_stream input,typename func,std::integral ch_type = typename input::char_type,std::size_t buffer_size=4096,bool randomaccess=false>
class itransform
{
public:
	using native_handle_type = input; 
	using transform_function_type = func;
	using char_type = ch_type;
	std::pair<native_handle_type,transform_function_type> handle;
	std::size_t position{},position_end{};
	std::array<char_type,buffer_size> buffer;
	auto& native_handle()
	{
		return handle.first;
	}
	auto& transform_function()
	{
		return handle.second;
	}
public:
	template<typename... Args>
	requires std::constructible_from<std::pair<native_handle_type,transform_function_type>,Args...>
	constexpr itransform(Args&& ...args):handle(std::forward<Args>(args)...){}
};


template<buffer_input_stream input,typename func,std::integral ch_type = typename input::char_type,std::size_t sz=4096,bool rac=false>
requires std::is_default_constructible_v<func>
class itransform_function_default_construct:public itransform<input,func,ch_type,sz,rac>
{
public:
	using native_handle_type = input; 
	using transform_function_type = func;
	using char_type = ch_type;
	template<typename... Args>
	requires std::constructible_from<native_handle_type,Args...>
	itransform_function_default_construct(Args&& ...args):itransform<input,func>(std::piecewise_construct,
				std::forward_as_tuple(std::forward<Args>(args)...),std::forward_as_tuple()){}
};
namespace details
{

template<typename T,std::contiguous_iterator Iter>
inline constexpr Iter itransform_read(T& ob,Iter cbegin,Iter cend)
{
/*	std::size_t diff(cend-cbegin);
	if(ob.buffer.size()<=ob.position+diff)[[unlikely]]
	{
		cbegin=std::copy_n(cbegin,ob.buffer.size()-ob.position,ob.buffer.data()+ob.position);
		for(ob.handle.second(ob.handle.first,ob.buffer.data(),ob.buffer.data()+ob.buffer.size());
			(cbegin=std::copy_n(cbegin,ob.buffer.size(),ob.buffer.data()))!=cend;
			ob.handle.second(ob.handle.first,ob.buffer.data(),ob.buffer.data()+ob.buffer.size()));

		return;
	}
	std::copy_n(cbegin,diff,ob.buffer.data()+ob.position);
	ob.position+=diff;*/
//Todo
	// empty ob first
	std::size_t remain_length(ob.position_end - ob.position);
	std::size_t available_length(cend - cbegin);
	if (remain_length < available_length)
		available_length = remain_length;
	cbegin = std::copy_n(ob.buffer.data() + ob.position, available_length, cbegin);
	//std::copy_n(ob.buffer.data() + ob.position + available_length, ob.position - available_length, ob.buffer.data());
	ob.position += available_length;

	// if buffer is empty now
	if (ob.position == ob.position_end)
	{
		auto read_pos(ob.handle.second.read_proxy(ob.handle.first, ob.buffer.data(), ob.buffer.data() + ob.buffer.size()));
		ob.position_end = read_pos - ob.buffer.data();
		ob.position = 0;
		remain_length = ob.position_end;
		available_length = cend - cbegin;
		if (remain_length < available_length)
			available_length = remain_length;
		cbegin = std::copy_n(ob.buffer.data(), available_length, cbegin);
		//std::copy_n(ob.buffer.data() + available_length, ob.position - available_length, ob.buffer.data());
		ob.position += available_length;
	}
	return cbegin;
}

}

template<buffer_input_stream input,typename func,std::integral ch_type,std::size_t sz,bool rac,std::contiguous_iterator Iter>
inline constexpr auto read(itransform<input,func,ch_type,sz,rac>& in,Iter cbegini,Iter cendi)
{
	return details::itransform_read(in,cbegini,cendi);
}

template<output_stream input,typename func,std::integral ch_type,std::size_t sz,bool rac,std::contiguous_iterator Iter>
inline constexpr auto write(itransform<input,func,ch_type,sz,rac>& ob,Iter cbegini,Iter cendi)
{
	return write(ob.handle.first,cbegini,cendi);
}

template<output_stream input,typename func,std::integral ch_type,std::size_t sz,bool rac>
inline constexpr void flush(itransform<input,func,ch_type,sz,rac>& ob)
{
//	flush(ob.handle.first);
}

template<buffer_input_stream input,typename func,std::integral ch_type,std::size_t sz,bool rac>
inline constexpr void iclear(itransform<input,func,ch_type,sz,rac>& in)
{
	in.position={};
}

template<buffer_input_stream input,typename func,std::integral ch_type,std::size_t sz,bool rac>
inline constexpr bool iflush(itransform<input,func,ch_type,sz,rac>& in)
{
	in.position_end=in.handle.second.read_proxy(in.handle.first,in.buffer.data(),in.buffer.data()+in.buffer.size())-in.buffer.data();
	in.position={};
	return in.position_end;
}

/*
template<output_stream input,typename func,std::integral ch_type,std::size_t sz,bool rac>
inline generator<ch_type> igenerator(itransform<input,func,ch_type,sz,rac>& in)
{
	for(;;)
	{
		for(;in.position!=in.position_end;++in.position)
			co_yield in.buffer[in.position];
		in.position_end=in.handle.second.read_proxy(in.handle.first,in.buffer.data(),in.buffer.data()+in.buffer.size())-in.buffer.data();
		in.position={};
		if(!in.position_end)
			break;
	}
}
*/
template<buffer_input_stream input,typename func,std::integral ch_type,std::size_t sz,bool rac>
inline constexpr auto begin(itransform<input,func,ch_type,sz,rac>& in)
{
	return in.buffer.data()+in.position;
}

template<buffer_input_stream input,typename func,std::integral ch_type,std::size_t sz,bool rac>
inline constexpr auto end(itransform<input,func,ch_type,sz,rac>& in)
{
	return in.buffer.data()+in.position_end;
}

template<buffer_input_stream input,typename func,std::integral ch_type,std::size_t sz,bool rac>
inline constexpr otransform<input,func>& operator++(itransform<input,func,ch_type,sz,rac>& in)
{
	++in.position;
	return in;
}

template<buffer_input_stream input,typename func,std::integral ch_type,std::size_t sz,bool rac,std::integral I>
inline constexpr otransform<input,func>& operator+=(itransform<input,func,ch_type,sz,rac>& in,I i)
{
	in.position+=i;
	return in;
}

template<output_stream input,typename func,std::integral ch_type,std::size_t sz,bool rac,std::integral I>
[[nodiscard]] inline constexpr auto oreserve(itransform<input,func,ch_type,sz,rac>& ob,I i)
{
	return oreserve(ob.handle.first,i);
}

template<buffer_output_stream input,typename func,std::integral ch_type,std::size_t sz,bool rac,std::integral I>
inline constexpr void orelease(itransform<input,func,ch_type,sz,rac>& ob,I i)
{
	orelease(ob.handle.first,i);
}

template<character_output_stream input,typename func,std::integral ch_type,std::size_t sz,bool rac>
inline constexpr void put(itransform<input,func,ch_type,sz,rac>& ob,typename itransform<input,func,ch_type,sz,rac>::char_type ch)
{
	put(ob.handle.first,ch);
}

template<buffer_input_stream input,typename func,std::integral ch_type,std::size_t sz,bool rac,typename... Args>
requires (random_access_stream<input>&&rac)
inline constexpr auto seek(itransform<input,func,ch_type,sz,rac>& in,Args&& ...args)
{
	in.position=in.position_end={};
	return in.seek_proxy(in.handle.first,std::forward<Args>(args)...);
}

namespace details
{
template<buffer_input_stream input,typename func,std::integral ch_type,std::size_t sz,bool rac>
constexpr bool itransform_ireserve_internal(itransform<input,func,ch_type,sz,rac>& ib,std::size_t n)
{
	if(ib.buffer.size()<=n)
#ifdef __cpp_exceptions
		throw std::system_error(EOPNOTSUPP,std::generic_category());
#else
		fast_terminate();
#endif
	ib.position=std::copy(ib.data()+ib.position,ib.data()+ib.position_end,ib.data())-ib.data();
	for(auto b(ib.position);;b=ib.position_end)
	{
		if(n<=(ib.position=ib.handle.second.read_proxy(ib.handle.first,ib.buffer.data(),ib.buffer.data()+ib.buffer.size())-ib.buffer.data()))
			return true;
		else if(!ib.position)
		{
			if(!b)
				return false;
			return true;	
		}
	}
}
}

template<buffer_input_stream input,typename func,std::integral ch_type,std::size_t sz,bool rac>
inline constexpr bool ireserve(itransform<input,func,ch_type,sz,rac>& ib,std::size_t n)
{
	if((sz-ib.position)<n)[[unlikely]]
		return details::itransform_ireserve_internal(ib,n);
	return true;
}
template<redirect_stream input,typename func,std::integral ch_type,std::size_t sz,bool rac>
inline constexpr decltype(auto) redirect_handle(itransform<input,func,ch_type,sz,rac>& t)
{
	return redirect_handle(t.native_handle());
}
}
