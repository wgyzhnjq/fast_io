#pragma once

namespace fast_io::win32
{

class nt_error:public std::exception
{
	std::uint32_t status;
public:
	nt_error(std::uint32_t val):status(val){}
	auto code() const
	{
		return status;
	}
	char const* what() const noexcept override
	{
		return "nt error";
	}
};


}