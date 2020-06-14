#pragma once

#include<iconv.h>

namespace fast_io
{

class posix_iconv_observer
{
public:
	using native_handle_type = iconv_t;
	iconv_t cd{(iconv_t) -1};
	constexpr decltype(auto) native_handle() const noexcept
	{
		return (cd);
	}
	constexpr decltype(auto) native_handle() noexcept
	{
		return (cd);
	}
	constexpr auto release() noexcept
	{
		auto temp{cd};
		cd = (iconv_t) -1;
		return temp;
	}
};

class posix_iconv:public posix_iconv_observer
{
public:
	using native_handle_type = iconv_t;
	constexpr posix_iconv()=default;
	constexpr posix_iconv(iconv_t cd):posix_iconv_observer{cd}{}
	posix_iconv(std::string_view to_code,std::string_view from_code):iconv_open(to_code.data(),from_code.data()){}
	posix_iconv(posix_iconv const&)=delete;
	posix_iconv& operator=(posix_iconv const&)=delete;
	constexpr posix_iconv(posix_iconv&& bmv) noexcept:posix_iconv_observer{bmv.native_handle()}
	{
		bmv.native_handle()=(iconv_t) -1;
	}
	posix_iconv& operator=(posix_iconv&& bmv) noexcept
	{
		if(std::addressof(bmv)==this)
			return *this;
		iconv_close(this->native_handle());
		this->native_handle()=bmv.native_handle();
		bmv.native_handle()=(iconv_t) -1;
		return *this;
	}
	~posix_iconv()
	{
		iconv_close(this->native_handle());
	}
};


}