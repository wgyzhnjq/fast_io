#pragma once

namespace fast_io
{

template<character_input_stream input>
inline constexpr bool scan_define(input& in,std::basic_string<typename input::char_type> &str)
{
	constexpr details::is_none_space dg;
	if constexpr(buffer_input_stream<input>)
	{
		for(;;)
		{
			decltype(auto) ibf_curr(ibuffer_curr(in));
			auto b{std::to_address(ibf_curr)};
			auto e{ibuffer_cend(in)};
			for(;b!=e&&!dg(*b);++b);
			ibf_curr=b;
			if(b==e)[[unlikely]]
			{
				if(!underflow(in))[[unlikely]]
					return false;
			}
			else
				break;
		}
		for(str.clear();;)
		{
			decltype(auto) ibf_curr(ibuffer_curr(in));
			auto b{std::to_address(ibf_curr)};
			auto e{ibuffer_cend(in)};
			auto i{b};
			for(;i!=e&&dg(*i);++i);
			str.append(b,i);
			ibf_curr=b;
			if(i==e)[[unlikely]]
			{
				if(!underflow(in))[[unlikely]]
					break;
			}
			else
				break;
		}
		return true;
	}
	else
	{
		auto gen{igenerator(in)};
		auto i{begin(gen)};
		auto e{end(gen)};
		for(;i!=e&&!dg(*i);++i);
		if(i==e)
			return false;
		for(str.clear();i!=e&&dg(*i);++i)
			str.push_back(*i);
		return true;
	}
}

template<input_stream input>
inline void scan_define(input& in,manip::whole<std::basic_string<typename input::char_type>> r)
{
	r.reference.clear();
	fast_io::basic_ostring_ref<std::basic_string<typename input::char_type>> ostrf(r.reference);
	transmit(ostrf,in);
}

template<buffer_input_stream input>
inline bool scan_define(input& in,manip::line<std::basic_string<typename input::char_type>> ref)
{
	auto gen{igenerator(in)};
	auto i{begin(gen)};
	auto e{end(gen)};
	if(i==e)
		return false;
	ref.reference.clear();
	for(;i!=e;++i)
	{
		if(*i==u8'\n')
		{
			++i;
			return true;
		}
		ref.reference.push_back(*i);
	}
	return true;
/*	auto i(begin(in));
	auto e(end(in));
	if(i==e)
		return false;
	for(ref.reference.clear();;)
	{
		auto i(begin(in));
		auto e(end(in));
		auto j(i);
		for(;j!=e&&*j!=u8'\n';++j);
		ref.reference.append(i,j);
		if(j!=e)
		{
			in+=j-i+1;
			return false;
		}
		if(!iflush(in))
			return false;
		ref.reference.reserve(ref.reference.size()<<1);
	}*/
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