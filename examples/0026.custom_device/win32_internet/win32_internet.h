#pragma once

namespace fast_io_examples
{


class win32_internet_handle
{
public:
	void* handle{};
	constexpr win32_internet_handle(){}
	constexpr win32_internet_handle(void* value):handle(value){}
	
	win32_internet_handle(win32_internet_handle&& bmv) noexcept:handle(bmv.handle)
	{
		bmv.handle={};
	}
	void close() noexcept
	{
		if(handle)
			InternetCloseHandle(handle);
	}
	win32_internet_handle& operator=(win32_internet_handle&& bmv) noexcept
	{
		if(bmv.handle==handle)
			return *this;
		close();
		handle=bmv.handle;
		bmv.handle={};
		return *this;
	}
	win32_internet_handle(win32_internet_handle const&) = delete;
	win32_internet_handle& operator=(win32_internet_handle const&) = delete;
	~win32_internet_handle()
	{
		close();
	}
};

template<typename... Args>
requires requires(Args&& ...args)
{
	InternetOpenA(std::forward<Args>(args)...);
}
inline win32_internet_handle win32_internet_open(Args&& ...args)
{
	auto ptr(InternetOpenA(std::forward<Args>(args)...));
	if(ptr==nullptr)
		throw fast_io::win32_error();
	return win32_internet_handle(ptr);
}


template<typename... Args>
requires requires(win32_internet_handle& handle,Args&& ...args)
{
	InternetConnectA(handle.handle,std::forward<Args>(args)...);
}
inline win32_internet_handle win32_internet_connect(win32_internet_handle& handle,Args&& ...args)
{
	auto ptr(InternetConnectA(handle.handle,std::forward<Args>(args)...));
	if(ptr==nullptr)
		throw fast_io::win32_error();
	return win32_internet_handle(ptr);
}

template<typename... Args>
requires requires(win32_internet_handle& handle,Args&& ...args)
{
	HttpOpenRequestA(handle.handle,std::forward<Args>(args)...);
}
inline win32_internet_handle win32_http_open_request(win32_internet_handle& handle,Args&& ...args)
{
	auto ptr(HttpOpenRequestA(handle.handle,std::forward<Args>(args)...));
	if(ptr==nullptr)
		throw fast_io::win32_error();
	return win32_internet_handle(ptr);
}

template<std::integral ch_type>
class basic_win32_internet_io_observer
{
public:
	using char_type = ch_type;
	using native_handle_type = void*;
	native_handle_type handle{};
	constexpr operator bool() noexcept
	{
		return handle;
	}
	constexpr auto& native_handle() noexcept
	{
		return handle;
	}
	constexpr auto& native_handle() const noexcept
	{
		return handle;
	};
};


template<std::integral ch_type,std::contiguous_iterator Iter>
inline Iter read(basic_win32_internet_io_observer<ch_type> iob,Iter begin,Iter end)
{
	unsigned long readed{};
	if(!InternetReadFile(iob.handle,std::to_address(begin),sizeof(*begin)*(end-begin),std::addressof(readed)))
		throw fast_io::win32_error();
	return begin+readed/sizeof(begin);
}

template<std::integral ch_type,std::contiguous_iterator Iter>
inline Iter write(basic_win32_internet_io_observer<ch_type> iob,Iter begin,Iter end)
{
	unsigned long written{};
	if(!InternetWriteFile(iob.handle,std::to_address(begin),sizeof(*begin)*(end-begin),std::addressof(written)))
		throw fast_io::win32_error();
	return begin+written/sizeof(begin);
}


using win32_internet_io_observer = basic_win32_internet_io_observer<char>;

}