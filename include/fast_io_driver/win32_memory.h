#pragma once
#include"../fast_io_hosted.h"
namespace fast_io
{

namespace win32
{
extern "C" void* __stdcall OpenProcess(std::uint32_t,int,std::uint32_t);
}

template<std::integral ch_type>
class basic_win32_memory_io_observer
{
public:
	using native_handle_type = void*;
	using base_address_type = std::uintptr_t;
	using char_type = ch_type;
	native_handle_type handle{};
	base_address_type base_addr{};
	constexpr native_handle_type release() noexcept
	{
		auto temp{handle};
		handle=nullptr;
		return temp;
	}
	constexpr operator bool() const noexcept
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
	constexpr auto& base_address() const noexcept
	{
		return base_addr;
	}
	constexpr auto& base_address() noexcept
	{
		return base_addr;
	}
};

template<std::integral ch_type>
class basic_win32_memory_io_handle: public basic_win32_memory_io_observer<ch_type>
{
public:
	using native_handle_type = basic_win32_memory_io_observer<ch_type>::native_handle_type;
	using base_address_type = basic_win32_memory_io_observer<ch_type>::base_address_type;
	using char_type = ch_type;
	constexpr basic_win32_memory_io_handle()=default;
	constexpr basic_win32_memory_io_handle(native_handle_type hd,base_address_type base):basic_win32_memory_io_observer<ch_type>(hd,base){}
	basic_win32_memory_io_handle(basic_win32_memory_io_handle const& other)
	{
		auto const current_process(win32::GetCurrentProcess());
		if(!win32::DuplicateHandle(current_process,other.native_handle(),current_process,std::addressof(this->native_handle()), 0, true, 2/*DUPLICATE_SAME_ACCESS*/))
#ifdef __cpp_exceptions
			throw win32_error();
#else
			fast_terminate();
#endif
		this->base_address()=other.base_address();
	}
	basic_win32_memory_io_handle& operator=(basic_win32_memory_io_handle const& other)
	{
		auto const current_process(win32::GetCurrentProcess());
		void* new_handle{};
		if(!win32::DuplicateHandle(current_process,other.native_handle(),current_process,std::addressof(new_handle), 0, true, 2/*DUPLICATE_SAME_ACCESS*/))
#ifdef __cpp_exceptions
			throw win32_error();
#else
			fast_terminate();
#endif
		if(this->native_handle())[[likely]]
			fast_io::win32::CloseHandle(this->native_handle());
		this->native_handle()=new_handle;
		this->base_address()=other.base_address();
		return *this;
	}
	basic_win32_memory_io_handle& operator=(basic_win32_memory_io_handle&& other) noexcept
	{
		if(other.native_handle()==this->native_handle())
			return *this;
		if(this->native_handle())[[likely]]
			fast_io::win32::CloseHandle(this->native_handle());
		this->native_handle()=other.native_handle();
		this->base_address()=other.base_address();
		other.native_handle()=nullptr;
		other.base_address()={};
		return *this;
	}
	base_win32_memory_io_handle(basic_win32_memory_io_handle&& other) noexcept:basic_win32_memory_io_observer<ch_type>(other.native_handle(),other.base_address())
	{
		other.native_handle()=nullptr;
		other.base_address()={};
	}
};

enum class win32_desired_access:std::uint32_t
{
delete = 0x00010000L,
read_control = 0x00020000L,
synchroize = 0x00100000L,
write_dac = 0x00040000L,
write_owner = 0x00080000L,
process_all_access = 0x1F0FFF,
process_create_process = 0x0080,
process_create_thread = 0x0002,
process_dup_handle = 0x0040,
process_query_information = 0x0400,
process_query_limited_information = 0x1000,
process_set_information = 0x0200,
process_set_quota = 0x0100,
process_suspend_resume = 0x0800,
process_terminate = 0x0001,
process_vm_operation = 0x0008,
process_vm_read = 0x0010,
process_vm_write = 0x0020,
synchroize = 0x00100000L
};

template<std::integral ch_type>
class basic_win32_memory_file: public basic_win32_memory_io_handle<ch_type>
{
public:
	using native_handle_type = basic_win32_memory_io_observer<ch_type>::native_handle_type;
	using base_address_type = basic_win32_memory_io_observer<ch_type>::base_address_type;
	using char_type = ch_type;
	constexpr basic_win32_memory_file()=default;
	constexpr basic_win32_memory_file(native_handle_type hd,base_address_type base):
		basic_win32_memory_io_handle<ch_type>(hd,base){}
	basic_win32_memory_file(win32_desired_access dw_desired_access,bool inherit_handle,std::uint32_t process_id,
		base_address_type base_addr={}):
		basic_win32_memory_io_handle<ch_type>(win32::OpenProcess(dw_desired_access,inherit_handle,process_id),base_addr)
	{
		if(this->native_handle()==nullptr)
			throw win32_error();
	}
	void close()
	{
		if(this->native_handle())[[likely]]
		{
			if(!fast_io::win32::CloseHandle(this->native_handle()))[[unlikely]]
				throw win32_error();
			this->native_handle()=nullptr;
			this->base_address()={};
		}
	}
	~basic_win32_memory_file()
	{
		if(this->native_handle())[[likely]]
			fast_io::win32::CloseHandle(this->native_handle());
	}
};

template<std::integral char_type,std::contiguous_iterator Iter>
inline [[nodiscard]] Iter read(basic_win32_memory_io_observer<char_type> iob,Iter begin,Iter end)
{
	std::size_t readed{};
	if(!ReadProcessMemory(iob.handle,bit_cast<void const*>(iob.base_addr),std::to_address(begin),(end-begin)*sizeof(*begin),std::addressof(readed)))
		throw win32_error();
	return begin+readed/sizeof(*begin);
}
template<std::integral char_type,std::contiguous_iterator Iter>
inline Iter write(basic_win32_memory_io_observer<char_type> iob,Iter begin,Iter end)
{
	std::size_t written{};
	if(!WriteProcessMemory(iob.handle,bit_cast<void const*>(iob.base_addr),
		std::to_address(begin),(end-begin)*sizeof(*begin),std::addressof(written)))
		throw win32_error();
	return begin+written/sizeof(*begin);
}
using win32_memory_io_observer = basic_win32_memory_io_observer<char>;
using win32_memory_io_handle = basic_win32_memory_io_handle<char>;
using win32_memory_file = basic_win32_memory_file<char>;
/*
template<std::integral char_type,std::contiguous_iterator Iter>
inline  seek(basic_win32_memory_io_observer<char_type> iob,Iter begin,Iter end)

static_assert(random_access_stream<win32_memory_io_observer>);
*/


}