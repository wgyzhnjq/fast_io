#pragma once

namespace fast_io
{

class nt_error:public std::exception
{
public:
	std::uint32_t code{};
	explicit nt_error(std::uint32_t status):std::exception(),status(code){}

	auto value() const noexcept
	{
		return code;
	}
	char const* what() const noexcept
	{
		
	}
};

}