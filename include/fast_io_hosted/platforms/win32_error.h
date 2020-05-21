#pragma once

namespace fast_io
{

//void * __stdcall LoadLibraryW(wchar_t const*);

/*
class win32_error : public std::runtime_error
{
	inline static std::string format_get_last_error(std::uint32_t error)
	{
		if (error)
		{
			std::array<char,32768> buffer;
			auto const buffer_length(win32::FormatMessageA(
			0x00000200 | 0x00001000,
			nullptr,
			error,
			(1 << 10),
			buffer.data(),
			buffer.size(),
			nullptr));
			if (buffer_length)
				return std::string(buffer.data(),buffer.data()+buffer_length);
		}
		return {};
	}
	std::uint32_t ec;
public:
	explicit win32_error(std::uint32_t error = win32::GetLastError()):std::runtime_error(format_get_last_error(error)),ec(error){}
	auto code() const noexcept
	{
		return ec;
	}
};
*/
class win32_error : public fast_io_error
{
	std::uint32_t ec;
public:
	explicit win32_error(std::uint32_t error = win32::GetLastError()):ec(error){}
	constexpr auto code() const noexcept
	{
		return ec;
	}
#if __cpp_constexpr >= 201907L
	constexpr
#endif
	void report(error_reporter& report) const override
	{
		constexpr std::size_t buffer_size{32768};
		reserve_write(report,buffer_size,[&](auto ptr)
		{
			auto const buffer_length(win32::FormatMessageA(
			0x00000200 | 0x00001000,
			nullptr,
			ec,
			(1 << 10),
			ptr,
			buffer_size,
			nullptr));
			return ptr+buffer_length;
		});
	}
};

namespace details
{
inline void throw_win32_error()
{
#ifdef __cpp_exceptions
	throw win32_error();
#else
	fast_terminate();
#endif
}
}
}