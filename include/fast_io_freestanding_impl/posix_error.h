#pragma once

namespace fast_io
{

class posix_error:public fast_io_error
{
	int ecd;
public:
	explicit posix_error(int errn=errno):ecd(errn){}
	constexpr auto code() const noexcept
	{
		return ecd;
	}
	/* FIO_CONSTEXPR */ void report(error_reporter& report) const override
	{
		print(report,std::string_view(strerror(ecd)));
	}
};

}