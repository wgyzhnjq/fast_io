#pragma once

#include <comdef.h>

namespace fast_io
{

class com_error:fast_io_error
{
public:
	HRESULT hresult{};
	constexpr auto code() const noexcept
	{
		return hresult;
	}
#if __cpp_constexpr >= 201907L
	constexpr
#endif
	void com_error(error_reporter& report) const override
	{
		_com_error ce(hresult);
		print(report,fast_io::code_cvt(std::string_view(ce.ErrorMessage())));
	}
};

}