#pragma once

namespace fast_io
{

template<buffer_input_stream input>
inline bool scan_define(input& in,std::basic_string<typename input::char_type> &str)
{
	details::is_none_space dg;
	auto sp(ispan(in));
	auto i(sp.data()),e(sp.data()+sp.size());
	using unsigned_char_type = std::make_unsigned_t<std::remove_cvref_t<decltype(*i)>>;
	for(;;)
	{
		for(;i!=e&&!dg(*i);++i);
		if(i!=e)
			break;
		if(!iflush(in))
			return false;
		sp=ispan(in);
		i=sp.data();
		e=sp.data()+sp.size();
	}
	for(str.clear();;)
	{
		for(;i!=e&&dg(*i);++i);
		str.append(sp.data(),i);
		if(i!=e)
		{
			icommit(in,i-sp.data());
			break;
		}
		if(!iflush(in))
			break;
		str.reserve(str.capacity()<<1);
		sp=ispan(in);
		i=sp.data();
		e=sp.data()+sp.size();
	}
	return true;
}

template<buffer_input_stream input>
inline void getwhole(input& in,std::basic_string<typename input::char_type> &str)
{
	for(str.clear();;)
	{
		auto sp(ispan(in));
		str.append(sp.data(),sp.data()+sp.size());
		if(!iflush(in))
			return;
		str.reserve(str.capacity()<<1);
	}
}

/*template<character_input_stream input>
inline void getline(input& in,std::basic_string<typename input::char_type> &str)
{
	str.clear();
	for(decltype(get<true>(in)) ch;!(ch=get<true>(in)).second&&ch.first!=0xA;str.push_back(ch.first));
}


template<character_input_stream input>
inline void getcarriage(input& in,std::basic_string<typename input::char_type> &str)
{
	str.clear();
	for(decltype(get<true>(in)) ch;!(ch=get<true>(in)).second&&ch.first!=0xD;str.push_back(ch.first));
}

template<character_input_stream input>
inline void getwhole(input& in,std::basic_string<typename input::char_type> &str)
{
	str.clear();
	for(decltype(get<true>(in)) ch;!(ch=get<true>(in)).second;str.push_back(ch.first));
}
*/
template<character_output_stream output,std::size_t indent_width,bool left,char8_t ch,typename T>
inline constexpr void print_define(output& out,manip::width<indent_width,left,ch,T const> a)
{
	basic_ostring<std::basic_string<typename output::char_type>> bas;
	print(bas,a.reference);
	std::size_t const size(bas.str().size());
	if(size<indent_width)
	{
		if constexpr(left)
		{
			print(out,bas.str());
			fill_nc(out,indent_width-size,ch);
		}	
		else
		{
			fill_nc(out,indent_width-size,ch);
			print(out,bas.str());
		}
	}
	else
		print(out,bas.str());
}

template<output_stream output>
inline constexpr void print_define(output& out,std::endian e)
{
	switch(e)
	{
	case std::endian::little:
		print(out,u8"little");
	break;
	case std::endian::big:
		print(out,u8"big");
	break;
	default:
		print(out,u8"unknown");
	}
}

}