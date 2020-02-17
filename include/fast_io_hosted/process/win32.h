#pragma once

namespace fast_io
{

struct win32_io_observer
{
public:
	void *handle=nullptr;
	using native_handle_type = void*;
	template<redirect_stream T>
	win32_io_observer(T& hd):handle(redirect_handle(hd)){}
	win32_io_observer()=default;
};

struct process_io
{
	win32_io_observer in,out,err;
};

namespace details
{
template<bool jn=false>
class basic_win32_process
{
	win32::process_information pinfo{};
public:
	using native_handle_type = win32::process_information;
	basic_win32_process(/*std::string_view path,*/
		std::string cmdline,
		process_io io)
	{
		win32::startupinfo sup{.cb=sizeof(win32::startupinfo),
		.dwFlags=0x00000100,//|0x00000001,
//		.wShowWindow=0x00000001,
		.hStdInput=io.in.handle,
		.hStdOutput=io.out.handle,
		.hStdError=io.err.handle};
		
		if(!win32::CreateProcessA(
			nullptr,cmdline.data(),
			nullptr,nullptr,true,
			0x00000080,
			nullptr,nullptr,std::addressof(sup),std::addressof(pinfo)))
			throw win32_error();
	}
	void detach()
	{
		if(pinfo.hThread)
		{
			win32::CloseHandle(pinfo.hThread);
			pinfo.hThread=nullptr;
		}
		if(pinfo.hProcess)
		{
			win32::CloseHandle(pinfo.hProcess);
			pinfo.hProcess=nullptr;
		}
	}
	inline auto& native_handle()
	{
		return pinfo;
	}
	inline auto const& native_handle() const
	{
		return pinfo;
	}
	void join()
	{
		if(static_cast<std::uint32_t>(0xFFFFFFFF)==win32::WaitForSingleObject(pinfo.hProcess,-1))
			throw win32_error();
		if constexpr(jn)
		{
			win32::CloseHandle(pinfo.hProcess);
			pinfo.hProcess={};
		}
	}
	basic_win32_process(basic_win32_process const&)=delete;
	basic_win32_process& operator=(basic_win32_process const&)=delete;
	basic_win32_process(basic_win32_process&& other) noexcept:pinfo(other.pinfo)
	{
		other.pinfo={};
	}
	basic_win32_process& operator=(basic_win32_process&& other) noexcept
	{
		if(std::addressof(other)!=this)
		{
			if constexpr(jn)
			{
				if(pinfo.hProcess)
					win32::WaitForSingleObject(pinfo.hProcess,-1);
			}
			detach();
			pinfo=other.pinfo;
			other.pinfo={};
		}
		return *this;
	}
	~basic_win32_process()
	{
		if constexpr(jn)
		{
			if(pinfo.hProcess)
				win32::WaitForSingleObject(pinfo.hProcess,-1);
		}
		detach();
	}
};
}

using win32_process = details::basic_win32_process<false>;
using win32_jprocess = details::basic_win32_process<true>;

}