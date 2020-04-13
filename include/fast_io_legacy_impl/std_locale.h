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
	if constexpr(std::same_as<std::iter_value_t<Iter>,stm::char_type>)
		return write(stm.handle,begin,end);
	else if constexpr(std::same_as<stm::char_type,char>)
	{
		decltype(auto) loc(stm.getloc());
		if(end-begin<=512)[[likely]]
		{
			std::array<wchar_t,512> array;
			return write(stm.handle,array.data(),
				loc.do_widen(std::to_address(begin),
					std::to_address(end),array.data()));
		}
		else
		{
			std::unique_ptr<wchar_t[]> uptr(new wchar_t[end-begin]);
			return write(stm.handle,uptr.get(),
				loc.do_widen(std::to_address(begin),
					std::to_address(end),uptr.get()));
		}

	}
	else
	{
		static_assert(std::same_as<stm::char_type,wchar_t>);
		decltype(auto) loc(stm.getloc());
		if(end-begin<=1024)[[likely]]
		{
			std::array<char,1024> array;
			return write(stm.handle,array.data(),
				loc.do_narrow(std::to_address(begin),
					std::to_address(end),array.data()));
		}
		else
		{
			std::unique_ptr<char[]> uptr(new char[end-begin]);
			return write(stm.handle,uptr.get(),
				loc.do_narrow(std::to_address(begin),
					std::to_address(end),uptr.data()));
		}
	}
}


template<input_stream stm,std::contiguous_iterator Iter>
requires ((std::same_as<stm::char_type,char>||std::same_as<stm::char_type,wchar_t>)&&
	(std::same_as<std::iter_value_t<Iter>,char>||std::same_as<std::iter_value_t<Iter>,wchar_t>))
inline Iter read(basic_cpp_locale_wrapper<stm>& stm,Iter begin,Iter end)
{
	if constexpr(std::same_as<std::iter_value_t<Iter>,stm::char_type>)
		return read(stm.handle,begin,end);
	else if constexpr(std::same_as<stm::char_type,char>)
	{
		decltype(auto) loc(stm.getloc());
		std::size_t sz{end-begin};
		if(sz<=1024)[[likely]]
		{
			std::array<char,1024> array;
			auto it{read(stm.handle,array.data(),array.data()+sz)};
			return loc.do_widen(array.data(),it,std::to_address(begin))-std::to_address(begin)+begin;
		}
		else
		{
			std::unique_ptr<char[]> uptr(new char[sz]);
			auto it{read(stm.handle,uptr.data(),uptr.data()+sz)};
			return loc.do_widen(uptr.data(),it,std::to_address(begin))-std::to_address(begin)+begin;
		}

	}
	else
	{
		static_assert(std::same_as<stm::char_type,wchar_t>);
		decltype(auto) loc(stm.getloc());
		std::size_t sz{end-begin};
		if(sz<=512)[[likely]]
		{
			std::array<wchar_t,512> array;
			auto it{read(stm.handle,array.data(),array.data()+sz)};
			return loc.do_narrow(array.data(),it,std::to_address(begin))-std::to_address(begin)+begin;
		}
		else
		{
			std::unique_ptr<wchar_t[]> uptr(new wchar_t[sz]);
			auto it{read(stm.handle,uptr.data(),uptr.data()+sz)};
			return loc.do_narrow(uptr.data(),it,std::to_address(begin))-std::to_address(begin)+begin;
		}
	}
}

}