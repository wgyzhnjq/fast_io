#pragma once

namespace fast_io
{

template<std::integral ch_type>
class basic_nt_io_handle
{
public:
	using char_type = ch_type;
	using native_handle_type = void*;
	native_handle_type handle{};
protected:
	void close_impl()
	{
		if(handle)
			::NtClose(handle);
	}
	basic_nt_io_handle() = default;
public:
	using char_type = ch_type;
	basic_nt_io_handle(native_handle_type hd):handle(hd){}
	native_handle_type& native_handle()
	{
		return handle;
	}
	inline void swap(basic_nt_io_handle& o) noexcept
	{
		using std::swap;
		swap(handle,o.handle);
	}
	constexpr void reset() noexcept
	{
		handle=nullptr;
	}
};
/*
template<std::integral ch_type,std::contiguous_iterator Iter>
inline Iter read(basic_nt_io_handle<ch_type>& hd,Iter begin,Iter end)
{
	if(::NtReadFile(hd.native_handle(),nullptr,{},{},{}))
	{

	}
}

template<std::integral ch_type,std::contiguous_iterator Iter>
inline Iter write(basic_nt_io_handle<ch_type>& hd,Iter begin,Iter end)
{
	
}

template<std::integral ch_type>
inline void flush(basic_nt_io_handle<ch_type>& hd)
{
	
}
*/
template<std::integral ch_type,bool kernel=false>
class basic_nt_file:public basic_nt_io_handle<ch_type>
{
public:
	using basic_nt_io_handle<ch_type>::native_handle_type;
	using basic_nt_io_handle<ch_type>::char_type;

	template<std::size_t om,perms pm>
	basic_nt_file(std::wstring_view filename,open::interface_t<om>,perms_interface_t<pm>)
	{
		UNICODE_STRING ustr{filename.size(),filename.size(),filename.data()};
		OBJECT_ATTRIBUTES oba{sizeof(OBJECT_ATTRIBUTES),nullptr,std::addressof(ustr)};
		if constexpr(kernel)
			oba.Attributes = 0x00000200;	//0x00000200
		if(::NtCreateFile(std::addressof(native_handle()),
			details::win32_file_openmode<om,pm>::mode.dwDesiredAccess,
			std::addressof(oba),
			nullptr,
			nullptr,
			))
		{

		}
	}
	~basic_nt_file()
	{
		this->close_impl();
	}
};
}