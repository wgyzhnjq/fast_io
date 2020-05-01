#pragma once

namespace fast_io::driver::openssl::details
{

template<typename T>
class observer
{
public:
	using native_handle_type = T;
	native_handle_type handle{};
	constexpr operator bool() noexcept
	{
		return handle;
	}
	constexpr auto& native_handle() const noexcept
	{
		return handle;
	}
	constexpr auto& native_handle() noexcept
	{
		return handle;
	}
};

}