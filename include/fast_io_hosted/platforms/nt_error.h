#pragma once

namespace fast_io
{

class nt_error:public win32_error
{
public:
	std::uint32_t code{};
	explicit nt_error(std::uint32_t status):win32_error(win32::nt::rtl_nt_status_to_dos_error(status)),code(status){}
	inline constexpr std::uint32_t nt_code() const noexcept
	{
		return code;
	}
	inline constexpr std::u8string_view message() const noexcept
	{
switch(code)
{
#include"nt_error_message.h"
}
	}
};


template<output_stream output>
inline constexpr void print_define(output& out,nt_error const& e)
{
	print(out,e.message());
}

}