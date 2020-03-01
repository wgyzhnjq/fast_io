#pragma once

namespace fast_io
{

template<std::integral T>
class basic_mfc_io_handle
{
	CFile* handle=nullptr;
public:
	using char_type = T;
	using native_handle_type = CFile*;
	basic_mfc_io_handle()=default;
	basic_mfc_io_handle(native_handle_type cf):handle(cf){}
	basic_mfc_io_handle(basic_mfc_io_handle const& mcf):handle(mcf.handle->Duplicate()){}
	basic_mfc_io_handle& operator=(basic_mfc_io_handle const& mcf)
	{
		auto temp{mcf.handle->Duplicate()};
		delete handle;
		handle=temp.handle;
		return *this;
	}
	basic_mfc_io_handle(basic_mfc_io_handle&& mcf) noexcept:handle(mcf.handle)
	{
		mcf.handle=nullptr;
	}
	basic_mfc_io_handle& operator=(basic_mfc_io_handle&& mcf) noexcept
	{
		if(mcf.handle!=handle)
		{
			delete handle;
			handle=mcf.handle;
			mcf.handle=nullptr;
		}
		return *this;
	}
	explicit operator basic_win32_io_handle<char_type>() const
	{
		return basic_win32_io_handle<char_type>(static_cast<void*>(handle));
	}
	native_handle_type native_handle() noexcept
	{
		return handle;
	}
	constexpr void reset() noexcept
	{
		handle=nullptr;
	}
};

template<std::integral T,std::contiguous_iterator Iter>
inline Iter write(basic_mfc_io_handle<T>& hd,Iter begin,Iter end)
{
	std::size_t const to_write{(end-begin)*sizeof(*begin)};
	if constexpr(sizeof(std::size_t)>4)
	{
		if (static_cast<std::size_t>(UINT32_MAX)<to_write)
		{
			hd.native_handle()->Write(std::to_address(begin),UINT32_MAX);
			return begin+(UINT32_MAX/sizeof(*begin));
		}
	}
	hd.native_handle()->Write(std::to_address(begin),static_cast<std::uint32_t>(to_write));
	return end;
}

template<std::integral T>
inline void flush(basic_mfc_io_handle<T>& hd)
{
	hd.native_handle()->Flush();
}

template<std::integral T,std::contiguous_iterator Iter>
inline Iter read(basic_mfc_io_handle<T>& hd,Iter begin,Iter end)
{
	std::size_t to_read{(end-begin)*sizeof(*begin)};
	if constexpr(sizeof(std::size_t)>4)
		if(static_cast<std::size_t>(UINT32_MAX)<to_read)
			to_read=static_cast<std::size_t>(UINT32_MAX);
	return begin+(hd.native_handle()->Read(std::to_address(begin),static_cast<std::uint32_t>(to_read))/sizeof(*begin));

}

template<std::integral T>
class basic_mfc_file:public basic_mfc_io_handle<T>
{
public:
	using char_type = T;
	using native_handle_type = CFile*;
	basic_mfc_file(native_interface_t,native_handle_type val):basic_mfc_io_handle<T>(val){}
	template<typename... Args>
	basic_mfc_file(basic_win32_io_handle<char_type>&& hd,Args&& ...):
		basic_mfc_io_handle<T>(new CFile(hd.native_handle()))
	{
		hd.reset();
	}
	template<open_mode om,typename... Args>
	basic_mfc_file(std::string_view file,open_interface_t<om>,Args&& ...args):
		basic_mfc_file(basic_win32_file<char_type>(file,open_interface<om>,std::forward<Args>(args)...),open_interface<om>)
	{}
	template<typename... Args>
	basic_mfc_file(std::string_view file,open_mode om,Args&& ...args):
		basic_mfc_file(basic_win32_file<char_type>(file,om,std::forward<Args>(args)...),om)
	{}
	template<typename... Args>
	basic_mfc_file(std::string_view file,std::string_view mode,Args&& ...args):
		basic_mfc_file(basic_win32_file<char_type>(file,mode,std::forward<Args>(args)...),mode)
	{}
	~basic_mfc_file()
	{
		delete this->native_handle();
	}
};

template<std::integral ch_type>
requires (redirect_stream<basic_win32_io_handle<ch_type>>)
inline decltype(auto) redirect_handle(basic_mfc_file<ch_type>& hd)
{
	return redirect_handle(static_cast<basic_win32_io_handle>(hd));
}
template<std::integral ch_type>
requires (zero_copy_in_stream<basic_win32_io_handle<ch_type>>)
inline decltype(auto) zero_copy_in_handle(basic_mfc_file<ch_type>& hd)
{
	return zero_copy_in_handle(static_cast<basic_win32_io_handle>(hd));
}
template<std::integral ch_type>
requires (zero_copy_out_stream<basic_win32_io_handle<ch_type>>)
inline decltype(auto) zero_copy_out_handle(basic_mfc_file<ch_type>& hd)
{
	return zero_copy_out_handle(static_cast<basic_win32_io_handle>(hd));
}

using mfc_io_handle=basic_mfc_io_handle<char>;
using mfc_file=basic_mfc_file<char>;
using u8mfc_io_handle=basic_mfc_io_handle<char8_t>;
using u8mfc_file=basic_mfc_file<char8_t>;


}