#pragma once

namespace fast_io
{
namespace details
{

template<char8_t base,my_unsigned_integral T>
inline constexpr void detect_overflow(T const& t,std::size_t length)
{
	constexpr std::size_t max_size{cal_max_int_size<T,base>()};
	if(max_size<=length)[[unlikely]]
	{
		if((max_size<length)|(t<=base))[[unlikely]]
			FIO_TEXT_ERROR("unsigned overflow");
	}
}

template<char8_t base,my_unsigned_integral T>
inline constexpr void detect_signed_overflow(T const& t,std::size_t length,bool sign)
{
	constexpr std::size_t max_size{cal_max_int_size<T,base>()};
	if(max_size<=length)[[unlikely]]
	{
		if((max_size<length)|(t<=base))[[unlikely]]
			FIO_TEXT_ERROR("signed overflow");
		if(static_cast<T>(get_int_max_unsigned<T>()+sign)<t)
			FIO_TEXT_ERROR("signed overflow");
	}
}


template<my_integral T,char8_t base,bool no_dec=false,character_input_stream input>
inline constexpr T input_base_number(input& in)
{
	using unsigned_char_type = std::make_unsigned_t<typename input::char_type>;
	using unsigned_t = my_make_unsigned_t<std::remove_cvref_t<T>>;
	if constexpr(my_unsigned_integral<T>)
	{
		if constexpr(sizeof(unsigned_t)==16)
		{
			//special optimization for uint128_t
			std::size_t length{};
			auto ig{igenerator(in)};
			auto it{begin(ig)};
			auto ed{end(ig)};
			std::uint64_t t{};
			for(;it!=ed;++it)
			{
				unsigned_char_type e(static_cast<unsigned_char_type>(*it)-u8'0');
				if constexpr(base<=10)
				{
					if(base<=e)[[unlikely]]
						break;
					t*=base;
					t+=e;
				}
				else
				{
					constexpr char8_t bm10{base-10};
					if(e<=10)
					{
						t*=base;
						t+=e;
					}
					else if(static_cast<unsigned_char_type>(e-=17)<bm10||static_cast<unsigned_char_type>(e-=32)<bm10)
					{
						t*=base;
						t+=e+10;
					}
					else[[unlikely]]
						break;
				}
				if(++length==19)[[unlikely]]
					break;
			}
			if(it!=ed&&length==19)	//phase 2
			{
				unsigned_t t2(t);
				for(++it;it!=ed;++it)
				{
					unsigned_char_type e(static_cast<unsigned_char_type>(*it)-u8'0');
					if constexpr(base<=10)
					{
						if(base<=e)[[unlikely]]
							break;
						t2*=base;
						t2+=e;
					}
					else
					{
						constexpr char8_t bm10{base-10};
						if(e<=10)
						{
							t2*=base;
							t2+=e;
						}
						else if(static_cast<unsigned_char_type>(e-=17)<bm10||static_cast<unsigned_char_type>(e-=32)<bm10)
						{
							t2*=base;
							t2+=e+10;
						}
						else[[unlikely]]
							break;
					}
					++length;
				}
				detect_overflow<base>(t2,length);
				return t2;
			};
			if(!length)[[unlikely]]
			{
				FIO_TEXT_ERROR("malformed input");
			}
			return t;
		}
		else
		{
			unsigned_t t{};
			std::size_t length{};
			for(unsigned_char_type ch : igenerator(in))
			{
				if constexpr(base<=10)
				{
					unsigned_char_type const e(static_cast<unsigned_char_type>(ch)-u8'0');
					if(base<=e)[[unlikely]]
						break;
					t*=base;
					t+=e;
				}
				else
				{
					unsigned_char_type e(static_cast<unsigned_char_type>(ch)-u8'0');
					constexpr char8_t bm10{base-10};
					if(e<=10)
					{
						t*=base;
						t+=e;
					}
					else if(static_cast<unsigned_char_type>(e-=17)<bm10||static_cast<unsigned_char_type>(e-=32)<bm10)
					{
						t*=base;
						t+=e+10;
					}
					else[[unlikely]]
						break;
				}
				++length;
			}

			if(!length)[[unlikely]]
				FIO_TEXT_ERROR("malformed input");
			detect_overflow<base>(t,length);
			return t;
		}
	}
	else
	{
		
		std::size_t length{};
		auto ig{igenerator(in)};
		auto it{begin(ig)};
		auto ed{end(ig)};
		if(it==ed)
			FIO_EOF();
		auto const sign{*it=='-'};
		if(sign)
			++it;
		if constexpr(sizeof(unsigned_t)==16)
		{
			std::uint64_t t{};
			for(;it!=ed;++it)
			{
				unsigned_char_type e(static_cast<unsigned_char_type>(*it)-u8'0');
				if constexpr(base<=10)
				{
					if(base<=e)[[unlikely]]
						break;
					t*=base;
					t+=e;
				}
				else
				{
					constexpr char8_t bm10{base-10};
					if(e<=10)
					{
						t*=base;
						t+=e;
					}
					else if(static_cast<unsigned_char_type>(e-=17)<bm10||static_cast<unsigned_char_type>(e-=32)<bm10)
					{
						t*=base;
						t+=e+10;
					}
					else[[unlikely]]
						break;
				}
				if(++length==19)[[unlikely]]
					break;
			}
			if(it!=ed&&length==19)	//phase 2
			{
				unsigned_t t2(t);
				for(++it;it!=ed;++it)
				{
					unsigned_char_type e(static_cast<unsigned_char_type>(*it)-u8'0');
					if constexpr(base<=10)
					{
						if(base<=e)[[unlikely]]
							break;
						t2*=base;
						t2+=e;
					}
					else
					{
						constexpr char8_t bm10{base-10};
						if(e<=10)
						{
							t2*=base;
							t2+=e;
						}
						else if(static_cast<unsigned_char_type>(e-=17)<bm10||static_cast<unsigned_char_type>(e-=32)<bm10)
						{
							t2*=base;
							t2+=e+10;
						}
						else[[unlikely]]
							break;
					}
					++length;
				}
				detect_signed_overflow<base>(t2,length,sign);
				if(sign)
					return -static_cast<T>(t2);
				else if(!length)[[unlikely]]
					FIO_TEXT_ERROR("malformed input");
				return static_cast<T>(t2);
			};
			if(sign)
				return -static_cast<T>(t);
			else if(!length)[[unlikely]]
				FIO_TEXT_ERROR("malformed input");
			return t;
		}
		else
		{
			unsigned_t t{};
			for(;it!=ed;++it)
			{
				if constexpr(base<=10)
				{
					unsigned_char_type const e(static_cast<unsigned_char_type>(*it)-u8'0');
					if(base<=e)[[unlikely]]
						break;
					t*=base;
					t+=e;
				}
				else
				{
					unsigned_char_type e(static_cast<unsigned_char_type>(*it)-u8'0');
					constexpr char8_t bm10{base-10};
					if(e<=10)
					{
						t*=base;
						t+=e;
					}
					else if(static_cast<unsigned_char_type>(e-=17)<bm10||static_cast<unsigned_char_type>(e-=32)<bm10)
					{
						t*=base;
						t+=e+10;
					}
					else[[unlikely]]
						break;
				}
				++length;
			}
			detect_signed_overflow<base>(t,length,sign);
			if(sign)
				return -static_cast<T>(t);
			else if(!length)[[unlikely]]
				FIO_TEXT_ERROR("malformed input");
			return static_cast<T>(t);
		}
	}
}
}

template<char8_t base,bool uppercase,character_input_stream input,details::my_integral T>
inline constexpr void space_scan_define(input& in,manip::base_t<base,uppercase,T> v)
{
	v.reference=details::input_base_number<std::remove_cvref_t<T>,base>(in);
}

template<character_input_stream input,details::my_integral T>
inline constexpr void space_scan_define(input& in,T& a)
{
	a=details::input_base_number<std::remove_cvref_t<T>,10>(in);
}

template<char8_t base,bool uppercase,character_input_stream input,typename T>
requires std::same_as<std::byte,std::remove_cvref_t<T>>
inline constexpr void space_scan_define(input& in,manip::base_t<base,uppercase,T> v)
{
	v.reference=static_cast<std::byte>(details::input_base_number<char8_t,base>(in));
}

template<character_input_stream input>
inline constexpr void space_scan_define(input& in,std::byte& a)
{
	a=std::byte(details::input_base_number<char8_t,10>(in));
}

/*
template<char8_t base,bool uppercase,character_input_stream input,typename T>
requires std::same_as<std::byte,std::remove_cvref_t<T>>
inline constexpr void scan_define(input& in,manip::no_decoration<manip::base_t<base,uppercase,T>> v)
{
	v.reference.reference=static_cast<std::byte>(details::input_base_number<char8_t,base>(in));
}

template<character_input_stream input,std::integral T>
inline constexpr void scan_define(input& in,manip::no_decoration<T> v)
{
	v.reference=details::input_base_number<std::remove_cvref_t<T>,10>(in);
}

template<char8_t base,bool uppercase,character_input_stream input,std::integral T>
inline constexpr void scan_define(input& in,manip::no_decoration<manip::base_t<base,uppercase,T>> v)
{
	v.reference.reference=details::input_base_number<std::remove_cvref_t<T>,base>(in);
}
*/
}
