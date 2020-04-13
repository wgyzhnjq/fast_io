#pragma once

#include<locale>

namespace fast_io
{

template<stream stm>
requires (std::same_as<stm::char_type,char>||std::same_as<stm::char_type,wchar_t>)
class basic_cpp_locale_wrapper
{
public:
	using char_type = typename stm::char_type;
	using native_handle_type = stm;
	using status_type = std::locale;
	native_handle_type handle;
	std::locale loc;
	auto& getloc() noexcept
	{
		return loc;
	}
	auto& getloc() const noexcept
	{
		return loc;
	}
	auto& native_handle() const noexcept
	{
		return handle;
	}
	auto& native_handle() noexcept
	{
		return handle;
	}
};

template<stream stm>
requires (std::same_as<stm::char_type,char>||std::same_as<stm::char_type,wchar_t>)&&requires(stm& sm)
{
	{sm.getloc()}->std::convertible_to<std::locale>;
}
class basic_cpp_locale_wrapper<stm>
{
public:
	using char_type = typename stm::char_type;
	using native_handle_type = stm;
	using status_type = std::locale;
	native_handle_type handle;
	decltype(auto) getloc()
	{
		return handle.getloc();
	}
	decltype(auto) getloc() const
	{
		return handle.getloc();
	}
	auto& native_handle() const noexcept
	{
		return handle;
	}
	auto& native_handle() noexcept
	{
		return handle;
	}
};

template<output_stream stm,std::contiguous_iterator Iter>
requires ((std::same_as<stm::char_type,char>||std::same_as<stm::char_type,wchar_t>)&&
	(std::same_as<std::iter_value_t<Iter>,char>||std::same_as<std::iter_value_t<Iter>,wchar_t>))
inline decltype(auto) write(basic_cpp_locale_wrapper<stm>& stm,Iter begin,Iter end)
{
	if constexpr(std::same_as<std::iter_value_t<Iter>,typename stm::char_type>)
		return write(stm.handle,begin,end);
	else if constexpr(std::same_as<stm::char_type,char>)
	{
		reserve_write(stm.handle,end-begin,[&](char* out_iter)
		{
			return stm.getloc().do_narrow(std::to_address(begin),std::to_address(end),
				out_iter);
		});
	}
	else
	{
		static_assert(std::same_as<stm::char_type,wchar_t>);
		reserve_write(stm.handle,end-begin,[&](wchar_t* out_iter)
		{
			return stm.getloc().do_widen(std::to_address(begin),std::to_address(end),
				out_iter);
		});
	}
}


template<input_stream stm,std::contiguous_iterator Iter>
requires ((std::same_as<stm::char_type,char>||std::same_as<stm::char_type,wchar_t>)&&
	(std::same_as<std::iter_value_t<Iter>,char>||std::same_as<std::iter_value_t<Iter>,wchar_t>))
inline Iter read(basic_cpp_locale_wrapper<stm>& stm,Iter begin,Iter end)
{
	if constexpr(std::same_as<std::iter_value_t<Iter>,typename stm::char_type>)
		return read(stm.handle,begin,end);
	else if constexpr(std::same_as<stm::char_type,char>)
	{
		reserve_read(stm.handle,end-begin,[&](char* inb,char* eb)
		{
			begin+=stm.getloc().do_widen(inb,eb,std::to_address(begin))-std::to_address(begin);
		});
		return begin;
	}
	else
	{
		static_assert(std::same_as<stm::char_type,wchar_t>);
		reserve_read(stm.handle,end-begin,[&](wchar_t* inb,wchar_t* eb)
		{
			begin+=stm.getloc().do_narrow(inb,eb,std::to_address(begin))-std::to_address(begin);
		});
		return begin;
	}
}

}