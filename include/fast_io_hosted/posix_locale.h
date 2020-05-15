#pragma once

namespace fast_io
{

enum class posix_locale_category:int
{
all=
#ifdef LC_ALL
LC_ALL
#else
0
#endif
,

collate=
#ifdef LC_COLLATE
LC_COLLATE
#else
0
#endif
,
ctype=
#ifdef LC_CTYPE
LC_CTYPE
#else
0
#endif
,
monetary=
#ifdef LC_MONETARY
LC_MONETARY
#else
0
#endif
,
numeric=
#ifdef LC_NUMERIC
LC_NUMERIC
#else
0
#endif
,
time=
#ifdef LC_TIME
LC_TIME
#else
0
#endif
,

address=
#ifdef LC_ADDRESS
LC_ADDRESS
#else
0
#endif
,

identification=
#ifdef LC_IDENTIFICATION
LC_IDENTIFICATION
#else
0
#endif
,

measurement=
#ifdef LC_MEASUREMENT
LC_MEASUREMENT
#else
0
#endif
,

messages=
#ifdef LC_MESSAGES
LC_MESSAGES
#else
0
#endif
,
name=
#ifdef LC_NAME
LC_NAME
#else
0
#endif
,
paper=
#ifdef LC_PAPER
LC_PAPER
#else
0
#endif
,
telephone=
#ifdef LC_TELEPHONE
LC_TELEPHONE
#else
0
#endif
,
xliterate=
#ifdef LC_XLITERATE
LC_XLITERATE
#else
0
#endif
,
keyboard=
#ifdef LC_KEYBOARD
LC_KEYBOARD
#else
0
#endif
,

none=0
};

constexpr posix_locale_category operator&(posix_locale_category x, posix_locale_category y) noexcept
{
using utype = typename std::underlying_type<posix_locale_category>::type;
return static_cast<posix_locale_category>(static_cast<utype>(x) & static_cast<utype>(y));
}

constexpr posix_locale_category operator|(posix_locale_category x, posix_locale_category y) noexcept
{
using utype = typename std::underlying_type<posix_locale_category>::type;
return static_cast<posix_locale_category>(static_cast<utype>(x) | static_cast<utype>(y));
}

constexpr posix_locale_category operator^(posix_locale_category x, posix_locale_category y) noexcept
{
using utype = typename std::underlying_type<posix_locale_category>::type;
return static_cast<posix_locale_category>(static_cast<utype>(x) ^ static_cast<utype>(y));
}

constexpr posix_locale_category operator~(posix_locale_category x) noexcept
{
using utype = typename std::underlying_type<posix_locale_category>::type;
return static_cast<posix_locale_category>(~static_cast<utype>(x));
}

inline constexpr posix_locale_category& operator&=(posix_locale_category& x, posix_locale_category y) noexcept{return x=x&y;}

inline constexpr posix_locale_category& operator|=(posix_locale_category& x, posix_locale_category y) noexcept{return x=x|y;}

inline constexpr posix_locale_category& operator^=(posix_locale_category& x, posix_locale_category y) noexcept{return x=x^y;}

#if defined(__WINNT__) || defined(_MSC_VER)
extern "C" lconv* localeconv_l(_locale_t) noexcept;
#endif

class posix_locale_observer
{
public:
	using native_handle_type = 
#if defined(__WINNT__) || defined(_MSC_VER)
_locale_t
#else
locale_t
#endif
	;
	native_handle_type loc{};
	constexpr auto& native_handle() const noexcept
	{
		return loc;
	}
	constexpr auto& native_handle() noexcept
	{
		return loc;
	}
	constexpr operator bool() const noexcept
	{
		return loc==static_cast<native_handle_type>(0);
	}
	auto release() noexcept
	{
		auto d{loc};
		loc=static_cast<native_handle_type>(0);
		return d;
	}
	auto get_lconv() noexcept
	{
		return localeconv_l(loc);
	}
};

class posix_locale_handle:public posix_locale_observer
{
public:
	using native_handle_type = posix_locale_observer::native_handle_type;
	constexpr posix_locale_handle()=default;
	constexpr posix_locale_handle(native_handle_type hd):posix_locale_observer(hd){}
	void close()
	{
	if(*this)[[likely]]
	{
#if defined(__WINNT__) || defined(_MSC_VER)
		_free_locale(this->native_handle());
#else
		freelocale(this->native_handle());
#endif
		this->native_handle()=static_cast<native_handle_type>(0);
	}		
	}
#if defined(__WINNT__) || defined(_MSC_VER)
	posix_locale_handle(posix_locale_handle const&)=delete;
	posix_locale_handle& operator=(posix_locale_handle const&)=delete;
#else
	posix_locale_handle(posix_locale_handle const& c):posix_locale_observer(duplocale(bmv.native_handle()))
	{
		if(!*this)
#ifdef __cpp_exceptions
			throw std::system_error(errno,std::generic_category());
#else
			fast_io::terminate();
#endif
	}
	posix_locale_handle& operator=(posix_locale_handle const& c)
	{
		auto cloned{duplocale(c.native_handle())};
		if(cloned==static_cast<native_handle_type>(0))
#ifdef __cpp_exceptions
			throw std::system_error(errno,std::generic_category());
#else
			fast_io::terminate();
#endif
		if(*this)[[likely]]
#if defined(__WINNT__) || defined(_MSC_VER)
			_free_locale(this->native_handle());
#else
			freelocale(this->native_handle());
#endif
		this->native_handle()=cloned;
		return *this;
	}
#endif

	constexpr posix_locale_handle(posix_locale_handle&& bmv) noexcept:posix_locale_observer(bmv.native_handle())
	{
		bmv.native_handle()=static_cast<native_handle_type>(0);
	}
	posix_locale_handle& operator=(posix_locale_handle&& bmv) noexcept
	{
		if(this->native_handle()==bmv.native_handle())
			return *this;
		
		if(*this)[[likely]]
#if defined(__WINNT__) || defined(_MSC_VER)
			_free_locale(this->native_handle());
#else
			freelocale(this->native_handle());
#endif
		this->native_handle()=bmv.native_handle();
		bmv.native_handle()=static_cast<native_handle_type>(0);
		return *this;
	}
};
class posix_locale:public posix_locale_handle
{
public:
	using native_handle_type = posix_locale_handle::native_handle_type;
	constexpr posix_locale()=default;
	constexpr posix_locale(native_handle_type hd):posix_locale_handle(hd){}
	posix_locale(posix_locale_category catg,std::string_view loc):
		posix_locale_handle(
#if defined(__WINNT__) || defined(_MSC_VER)
	_create_locale(static_cast<int>(catg),loc.data())
#else
	newlocale(static_cast<int>(catg),loc.data(),static_cast<locale_t>(0))
#endif
)
{
	if(!*this)
#ifdef __cpp_exceptions
		throw std::system_error(errno,std::generic_category());
#else
		fast_io::terminate();
#endif
}
	~posix_locale()
	{
	if(*this)[[likely]]
#if defined(__WINNT__) || defined(_MSC_VER)
		_free_locale(this->native_handle());
#else
		freelocale(this->native_handle());
#endif
	}
};

}