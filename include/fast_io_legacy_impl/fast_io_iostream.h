#pragma once

namespace fast_io
{

template<stream hd_type,typename traits_tp = std::char_traits<typename hd_type::char_type>>
class fast_io_stream_base
{
public:
	using native_handle_type = hd_type;
	using traits_type = traits_tp;
	using char_type = typename traits_type::char_type;
	using streambuf_type = fast_io_basic_streambuf<native_handle_type,traits_tp>;
private:
	streambuf_type handle;
public:
	template<typename... Args>
	requires std::constructible_from<native_handle_type,Args...>
	fast_io_stream_base(Args&& ...args):handle(std::forward<Args>(args)...){}
	constexpr auto& streambuf_handle()
	{
		return handle;
	}
	constexpr auto& native_handle()
	{
		return handle.native_handle();
	}
};

template<stream hd_type,typename streamtype>
requires std::same_as<typename hd_type::char_type,typename streamtype::char_type>
class fast_io_basic_stream final:public fast_io_stream_base<hd_type,typename streamtype::traits_type>,
public streamtype
{
public:
	using native_handle_type = hd_type;
	using stream_type = streamtype;
	using traits_type = typename stream_type::traits_type;
	using char_type = typename stream_type::char_type;
	using streambuf_type = fast_io_basic_streambuf<native_handle_type,traits_type>;
	using stream_base_type = fast_io_stream_base<native_handle_type,traits_type>;
	template<typename... Args>
	requires std::constructible_from<native_handle_type,Args...>
	fast_io_basic_stream(Args&& ...args):
		stream_base_type(std::forward<Args>(args)...),
		stream_type(std::addressof(this->streambuf_handle())){}
};


template<input_stream hd_type,typename traits_type = std::char_traits<typename hd_type::char_type>>
using fast_io_istream=fast_io_basic_stream<hd_type,std::basic_istream<typename traits_type::char_type,traits_type>>;

template<output_stream hd_type,typename traits_type = std::char_traits<typename hd_type::char_type>>
using fast_io_ostream=fast_io_basic_stream<hd_type,std::basic_ostream<typename traits_type::char_type,traits_type>>;

template<io_stream hd_type,typename traits_type = std::char_traits<typename hd_type::char_type>>
using fast_io_iostream=fast_io_basic_stream<hd_type,std::basic_iostream<typename traits_type::char_type,traits_type>>;

template<stream hd_type>
using iostream_type=std::conditional_t<io_stream<hd_type>,fast_io_iostream<hd_type>,
std::conditional_t<input_stream<hd_type>,fast_io_istream<hd_type>,fast_io_ostream<hd_type>>>;

template<stream hd_type>
using iostream_reference_type=std::conditional_t<io_stream<hd_type>,fast_io_iostream<io_ref<hd_type>>,
std::conditional_t<input_stream<hd_type>,
fast_io_istream<io_ref<hd_type>>,
fast_io_ostream<io_ref<hd_type>>
>>;

template<stream hd_type>
inline iostream_reference_type<hd_type> iostream_ref(hd_type& ref)
{
	return ref;
}


}