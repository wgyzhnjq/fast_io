#pragma once

namespace fast_io
{
	
namespace details
{

template<typename>
struct text_view_interal_variable{};

template<character_input_stream T>
struct text_view_interal_variable<T>
{
	bool state=false;
	typename T::char_type internal_character = 0;
};

}

template<typename T>
requires character_input_stream<T>||character_output_stream<T>
class text_view
{
public:
	T ib;
	details::text_view_interal_variable<T> state;
public:
	using native_interface_t = T;
	using char_type = typename native_interface_t::char_type;
public:
	template<typename ...Args>
	requires std::constructible_from<T,Args...>
	constexpr text_view(Args&& ...args):ib(std::forward<Args>(args)...){}
	constexpr inline auto& native_handle()
	{
		return ib;
	}
	constexpr inline char_type mmget() requires character_input_stream<T>
	{
		if(state.state)
		{
			state.state=false;
			return state.internal_character;
		}
		auto ch(get(ib));
		if(ch==0xD)
		{
			auto internal(try_get(ib));
			if(internal.second)
				return ch;
			if(internal.first==0xA)
				return 0xA;
			state.state=true;
			state.internal_character=internal.first;
		}
		return ch;
	}
	constexpr inline std::pair<char_type,bool> mmtry_get() requires character_input_stream<T>
	{
		if(state.state)
		{
			state.state=false;
			return {state.internal_character,false};
		}
		auto ch(try_get(ib));
		if(ch.second)
			return {0,true};
		if(ch.first==0xD)
		{
			auto internal(try_get(ib));
			if(internal.second)
				return ch;
			if(internal.first==0xA)
				return internal;
			state.state=true;
			state.internal_character=internal.first;
		}
		return ch;
	}
	template<std::contiguous_iterator Iter>
	constexpr inline Iter mmreceive(Iter b,Iter e)
		requires character_input_stream<T>
	{
		auto pb(static_cast<char_type*>(static_cast<void*>(std::to_address(b))));
		auto pe(pb+(e-b)*sizeof(*b)/sizeof(char_type));
		auto pi(pb);
		for(;pi!=pe;++pi)
			*pi=mmget();
		return b+(pi-pb)*sizeof(char_type)/sizeof(*b);
	}
};

template<stream srm>
text_view(srm&&) -> text_view<srm>;

template<character_input_stream T>
constexpr inline auto get(text_view<T>& input)
{
	return input.mmget();
}

template<character_input_stream T>
constexpr inline auto try_get(text_view<T>& input)
{
	return input.mmtry_get();
}

template<character_input_stream T,std::contiguous_iterator Iter>
constexpr inline Iter receive(text_view<T>& input,Iter b,Iter e)
{
	return input.mmreceive(b,e);
}

template<character_output_stream T>
constexpr inline void put(text_view<T>& output,typename text_view<T>::char_type ch)
{
	if(ch==0xA)
		put(output.ib,0xD);
	put(output.ib,ch);
}

template<character_output_stream T,std::contiguous_iterator Iter>
constexpr inline void send(text_view<T>& output,Iter b,Iter e)
{
//	output.mmsend(b,e);
	using char_type = T::char_type;
	auto pb(static_cast<char_type const*>(static_cast<void const*>(std::to_address(b))));
	auto last(pb);
	auto pi(pb),pe(pb+(e-b)*sizeof(*b)/sizeof(char_type));
	for(;pi!=pe;++pi)
		if(*pi==0xA)
		{
			if(last!=pi)
				send(output.ib,last,pi-1);
			put(output.ib,0xD);
			put(output.ib,0xA);
			last=pi+1;
		}
	send(output.ib,last,pe);
}

template<character_output_stream T>
constexpr inline void flush(text_view<T>& output)
{
	flush(output.ib);
}

template<stream T>
inline constexpr void fill_nc(text_view<T>& view,std::size_t count,typename T::char_type const& ch)
{
	if(ch==0xA)
	{
		for(std::size_t i(0);i!=count;++i)
		{
			put(view.ib,0xD);
			put(view.vs,0xA);
		}
		return;
	}
	fill_nc(view.ib,count,ch);
}


}