#pragma once

namespace fast_io
{
namespace win32::nt
{

inline auto get_nt_module_handle()
{
	auto mod(GetModuleHandleW(L"ntdll.dll"));
	if(mod==nullptr)
#ifdef __cpp_exceptions
		throw win32_error();
#else
		fast_terminate();
#endif
	return mod;
}

template<typename func>
inline auto get_nt_module_handle(char const* funname)
{
	auto proc_addr(GetProcAddress(get_nt_module_handle(),funname));
	if(proc_addr==nullptr)
#ifdef __cpp_exceptions
		throw win32_error();
#else
		fast_terminate();
#endif
	return bit_cast<func*>(proc_addr);
}

inline std::uint32_t rtl_nt_status_to_dos_error(std::uint32_t status)
{
	return (get_nt_module_handle<std::uint32_t __stdcall(std::uint32_t status)>("RtlNtStatusToDosError"))(status);
}

}

class nt_error:public win32_error
{
public:
	std::uint32_t code{};
	explicit nt_error(std::uint32_t status):win32_error(win32::nt::rtl_nt_status_to_dos_error(status)),code(status){}
	inline constexpr std::uint32_t nt_code()
	{
		return code;
	}
};

namespace details
{

inline constexpr std::u8string_view get_nt_message(nt_error const& e) noexcept
{
switch(e.code)
{
#include"nt_error_message.h"
}
}
}

template<output_stream output>
inline constexpr void print_define(output& out,nt_error const& e)
{
	print(out,details::get_nt_message(e));
}

}