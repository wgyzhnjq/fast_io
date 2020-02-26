#pragma once

#include"win32_error.h"

namespace fast_io
{

namespace details
{

template<bool inherit=false>
inline void* create_file_a_impl(char const* lpFileName,
std::uint32_t dwDesiredAccess,
std::uint32_t dwShareMode,
std::uint32_t dwCreationDisposition,
std::uint32_t dwFlagsAndAttributes)
{
	if constexpr(inherit)
	{
		win32::security_attributes sec_attr
		{
			.nLength=sizeof(win32::security_attributes),
			.bInheritHandle = true
		};
		auto handle(win32::CreateFileA(lpFileName,
		dwDesiredAccess,
		dwShareMode,
		std::addressof(sec_attr),
		dwCreationDisposition,
		dwFlagsAndAttributes,
		nullptr));
		if(handle==((void*) (std::intptr_t)-1))
#ifdef __cpp_exceptions
			throw win32_error();
#else
			fast_terminate();
#endif
		return handle;
	}
	else
	{
		auto handle(win32::CreateFileA(lpFileName,
		dwDesiredAccess,
		dwShareMode,
		nullptr,
		dwCreationDisposition,
		dwFlagsAndAttributes,
		nullptr));
		if(handle==((void*) (std::intptr_t)-1))
#ifdef __cpp_exceptions
			throw win32_error();
#else
			fast_terminate();
#endif
		return handle;
	}
}

struct win32_open_mode
{
std::uint32_t dwDesiredAccess{};
std::uint32_t dwShareMode{1|2};//FILE_SHARE_READ|FILE_SHARE_WRITE
std::uint32_t dwCreationDisposition{};	//depends on EXCL
std::uint32_t dwFlagsAndAttributes{};//=128|0x10000000;//FILE_ATTRIBUTE_NORMAL|FILE_FLAG_RANDOM_ACCESS
};

inline constexpr win32_open_mode calculate_win32_open_mode(open_mode value)
{
	value&=~open_mode::ate;
	win32_open_mode mode;
	if((value&open_mode::app)!=open_mode::none)
		mode.dwDesiredAccess|=4;//FILE_APPEND_DATA
	else if((value&open_mode::out)!=open_mode::none)
		mode.dwDesiredAccess|=0x40000000;//GENERIC_WRITE
	if((value&open_mode::in)!=open_mode::none)
		mode.dwDesiredAccess|=0x80000000;//GENERIC_READ
	if((value&open_mode::excl)!=open_mode::none)
	{
		mode.dwCreationDisposition=1;//	CREATE_NEW
		if((value&open_mode::trunc)!=open_mode::none)
#ifdef __cpp_exceptions
			throw std::system_error(make_error_code(std::errc::invalid_argument));
#else
			fast_terminate();
#endif
	}
	else if ((value&open_mode::trunc)!=open_mode::none)
		mode.dwCreationDisposition=2;//CREATE_ALWAYS
	else if((value&open_mode::in)==open_mode::none)
	{
		if((value&open_mode::app)==open_mode::none)
			mode.dwCreationDisposition=4;//OPEN_ALWAYS
		else
			mode.dwCreationDisposition=2;//CREATE_ALWAYS
	}
	else
		mode.dwCreationDisposition=3;//OPEN_EXISTING
	if((value&open_mode::direct)!=open_mode::none)
		mode.dwFlagsAndAttributes|=0x20000000;//FILE_FLAG_NO_BUFFERING
	if((value&open_mode::sync)!=open_mode::none)
		mode.dwFlagsAndAttributes|=0x80000000;//FILE_FLAG_WRITE_THROUGH
	if((value&open_mode::no_block)!=open_mode::none)
		mode.dwFlagsAndAttributes|=0x40000000;//FILE_FLAG_OVERLAPPED
	if((value&open_mode::follow)!=open_mode::none)
		mode.dwFlagsAndAttributes|=0x00200000;	//FILE_FLAG_OPEN_REPARSE_POINT
	if((value&open_mode::directory)!=open_mode::none)
		mode.dwFlagsAndAttributes|=0x02000000;	//FILE_FLAG_BACKUP_SEMANTICS
	bool set_normal{true};
	if((value&open_mode::archive)!=open_mode::none)
	{
		mode.dwFlagsAndAttributes|=0x20;		//FILE_ATTRIBUTE_ARCHIVE
		set_normal={};
	}
	if((value&open_mode::encrypted)!=open_mode::none)
	{
		mode.dwFlagsAndAttributes|=0x4000;		//FILE_ATTRIBUTE_ENCRYPTED
		set_normal={};
	}
	if((value&open_mode::hidden)!=open_mode::none)
	{
		mode.dwFlagsAndAttributes|=0x2;			//FILE_ATTRIBUTE_HIDDEN
		set_normal={};
	}
	if((value&open_mode::compressed)!=open_mode::none)
	{
		mode.dwFlagsAndAttributes|=0x800;		//FILE_ATTRIBUTE_COMPRESSED
		set_normal={};
	}
	if((value&open_mode::system)!=open_mode::none)
	{
		mode.dwFlagsAndAttributes|=0x4;							//FILE_ATTRIBUTE_SYSTEM
		set_normal={};
	}
	if((value&open_mode::offline)!=open_mode::none)
	{
		mode.dwFlagsAndAttributes|=0x1000;						//FILE_ATTRIBUTE_OFFLINE
		set_normal={};
	}
	if((value&open_mode::directory)!=open_mode::none)
	{
		mode.dwFlagsAndAttributes|=0x10;						//FILE_ATTRIBUTE_DIRECTORY
		set_normal={};
	}
	if(set_normal)[[likely]]
		mode.dwFlagsAndAttributes|=0x80;						//FILE_ATTRIBUTE_NORMAL
	if((value&open_mode::sequential_scan)==open_mode::none)
		mode.dwFlagsAndAttributes|=0x10000000;		//FILE_FLAG_RANDOM_ACCESS
	else
		mode.dwFlagsAndAttributes|=0x08000000;		//FILE_FLAG_SEQUENTIAL_SCAN
	if((value&open_mode::no_recall)!=open_mode::none)
		mode.dwFlagsAndAttributes|=0x00100000;					//FILE_FLAG_OPEN_NO_RECALL
	if((value&open_mode::posix_semantics)!=open_mode::none)
		mode.dwFlagsAndAttributes|=0x01000000;					//FILE_FLAG_POSIX_SEMANTICS
	if((value&open_mode::session_aware)!=open_mode::none)
		mode.dwFlagsAndAttributes|=0x00800000;					//FILE_FLAG_SESSION_AWARE

	return mode;
}

inline constexpr std::uint32_t dw_flag_attribute_with_perms(std::uint32_t dw_flags_and_attributes,perms pm)
{
	if((pm&perms::owner_write)==perms::none)
		return dw_flags_and_attributes|1;//dw_flags_and_attributes|FILE_ATTRIBUTE_READONLY
	return dw_flags_and_attributes;
}

inline constexpr win32_open_mode calculate_win32_open_mode_with_perms(open_mode om,perms pm)
{
	auto m(calculate_win32_open_mode(om));
	m.dwFlagsAndAttributes=dw_flag_attribute_with_perms(m.dwFlagsAndAttributes,pm);
	return m;
}

template<open_mode om,perms pm>
struct win32_file_openmode
{
	inline static constexpr win32_open_mode mode = calculate_win32_open_mode_with_perms(om,pm);
};

template<open_mode om>
struct win32_file_openmode_single
{
	inline static constexpr win32_open_mode mode = calculate_win32_open_mode(om);
};
}

template<std::integral ch_type>
class basic_win32_io_handle
{
public:
	using native_handle_type = void*;
private:
	native_handle_type mhandle;
protected:
	void close_impl()
	{
		if(mhandle)
			fast_io::win32::CloseHandle(mhandle);
	}
	basic_win32_io_handle() = default;
public:
	using char_type = ch_type;
	basic_win32_io_handle(native_handle_type handle):
		mhandle(handle){}
	basic_win32_io_handle(std::uint32_t dw):
		mhandle(fast_io::win32::GetStdHandle(dw)){}
	void close()
	{
		close_impl();
		mhandle=nullptr;
	}
	basic_win32_io_handle(basic_win32_io_handle const& other)
	{
		auto const current_process(win32::GetCurrentProcess());
		if(!win32::DuplicateHandle(current_process,other.mhandle,current_process,std::addressof(mhandle), 0, true, 2/*DUPLICATE_SAME_ACCESS*/))
#ifdef __cpp_exceptions
			throw win32_error();
#else
			fast_terminate();
#endif
	}
	basic_win32_io_handle& operator=(basic_win32_io_handle const& other)
	{
		auto const current_process(win32::GetCurrentProcess());
		void* new_handle{};
		if(!win32::DuplicateHandle(current_process,other.mhandle,current_process,std::addressof(new_handle), 0, true, 2/*DUPLICATE_SAME_ACCESS*/))
#ifdef __cpp_exceptions
			throw win32_error();
#else
			fast_terminate();
#endif
		close_impl();
		mhandle=new_handle;
		return *this;
	}
	basic_win32_io_handle(basic_win32_io_handle&& b) noexcept:mhandle(b.mhandle)
	{
		b.mhandle=nullptr;
	}
	basic_win32_io_handle& operator=(basic_win32_io_handle&& b) noexcept
	{
		if(std::addressof(b)!=this)
		{
			close_impl();
			mhandle = b.mhandle;
			b.mhandle=nullptr;
		}
		return *this;
	}
	native_handle_type& native_handle()
	{
		return mhandle;
	}
	inline void swap(basic_win32_io_handle& o) noexcept
	{
		using std::swap;
		swap(mhandle,o.mhandle);
	}
	constexpr void reset() noexcept
	{
		mhandle=nullptr;
	}
};

template<std::integral ch_type>
inline bool valid(basic_win32_io_handle<ch_type>& hd)
{
	return hd.native_handle();
}

template<std::integral ch_type>
inline auto redirect_handle(basic_win32_io_handle<ch_type>& hd)
{
	return hd.native_handle();
}

template<std::integral ch_type>
inline void swap(basic_win32_io_handle<ch_type>& a,basic_win32_io_handle<ch_type>& b) noexcept
{
	a.swap(b);
}

template<std::integral ch_type,typename T,std::integral U>
inline std::common_type_t<std::int64_t, std::size_t> seek(basic_win32_io_handle<ch_type>& handle,seek_type_t<T>,U i=0,seekdir s=seekdir::cur)
{
	std::int64_t distance_to_move_high{};
	std::int64_t seekposition{seek_precondition<std::int64_t,T,ch_type>(i)};
	if(!win32::SetFilePointerEx(handle.native_handle(),seekposition,std::addressof(distance_to_move_high),static_cast<std::uint32_t>(s)))
#ifdef __cpp_exceptions
		throw win32_error();
#else
		fast_terminate();
#endif
	return distance_to_move_high;
}

template<std::integral ch_type,std::integral U>
inline auto seek(basic_win32_io_handle<ch_type>& handle,U i=0,seekdir s=seekdir::cur)
{
	return seek(handle,seek_type<ch_type>,i,s);
}

template<std::integral ch_type,std::contiguous_iterator Iter>
inline Iter read(basic_win32_io_handle<ch_type>& handle,Iter begin,Iter end)
{
	std::uint32_t numberOfBytesRead{};
	if(!win32::ReadFile(handle.native_handle(),std::to_address(begin),static_cast<std::uint32_t>((end-begin)*sizeof(*begin)),std::addressof(numberOfBytesRead),nullptr))
	{
		auto err(win32::GetLastError());
		if(err==109)
			return begin;
#ifdef __cpp_exceptions
		throw win32_error(err);
#else
		fast_terminate();
#endif
	}
	return begin+numberOfBytesRead;
}
template<std::integral ch_type,std::contiguous_iterator Iter>
inline Iter write(basic_win32_io_handle<ch_type>& handle,Iter cbegin,Iter cend)
{
	auto nNumberOfBytesToWrite(static_cast<std::uint32_t>((cend-cbegin)*sizeof(*cbegin)));
	std::uint32_t numberOfBytesWritten;
	if(!win32::WriteFile(handle.native_handle(),std::to_address(cbegin),nNumberOfBytesToWrite,std::addressof(numberOfBytesWritten),nullptr))
#ifdef __cpp_exceptions
		throw win32_error();
#else
		fast_terminate();
#endif
	return cbegin+numberOfBytesWritten/sizeof(*cbegin);
}
template<std::integral ch_type>
inline constexpr void flush(basic_win32_io_handle<ch_type>&){}

template<std::integral ch_type>
class basic_win32_file:public basic_win32_io_handle<ch_type>
{
public:
	using char_type=ch_type;
	using native_handle_type = basic_win32_io_handle<ch_type>::native_handle_type;
	using basic_win32_io_handle<ch_type>::native_handle;
	template<typename ...Args>
	requires requires(Args&& ...args)
	{
		{win32::CreateFileW(std::forward<Args>(args)...)}->std::same_as<native_handle_type>;
	}
	basic_win32_file(fast_io::native_interface_t,Args&& ...args):basic_win32_io_handle<char_type>(win32::CreateFileW(std::forward<Args>(args)...))
	{
		if(native_handle()==((void*) (std::intptr_t)-1))
#ifdef __cpp_exceptions
			throw win32_error();
#else
			fast_terminate();
#endif
	}

	template<open_mode om,perms pm>
	basic_win32_file(std::string_view filename,open_interface_t<om>,perms_interface_t<pm>):
				basic_win32_io_handle<char_type>(
				details::create_file_a_impl<(om&open_mode::inherit)!=open_mode::none>(filename.data(),
				details::win32_file_openmode<om,pm>::mode.dwDesiredAccess,
				details::win32_file_openmode<om,pm>::mode.dwShareMode,
				details::win32_file_openmode<om,pm>::mode.dwCreationDisposition,
				details::win32_file_openmode<om,pm>::mode.dwFlagsAndAttributes)
				)
	{
		if constexpr ((om&open_mode::ate)!=open_mode::none)
			seek(*this,0,seekdir::end);
	}
	template<open_mode om>
	basic_win32_file(std::string_view filename,open_interface_t<om>):basic_win32_io_handle<char_type>(
				details::create_file_a_impl<(om&open_mode::inherit)!=open_mode::none>(filename.data(),
				details::win32_file_openmode_single<om>::mode.dwDesiredAccess,
				details::win32_file_openmode_single<om>::mode.dwShareMode,
				details::win32_file_openmode_single<om>::mode.dwCreationDisposition,
				details::win32_file_openmode_single<om>::mode.dwFlagsAndAttributes))
	{
		if constexpr ((om&open_mode::ate)!=open_mode::none)
			seek(*this,0,seekdir::end);
	}
	template<open_mode om>
	basic_win32_file(std::string_view filename,open_interface_t<om>,perms p):basic_win32_io_handle<char_type>(
				details::create_file_a_impl<(om&open_mode::inherit)!=open_mode::none>(filename.data(),
				details::win32_file_openmode_single<om>::mode.dwDesiredAccess,
				details::win32_file_openmode_single<om>::mode.dwShareMode,
				details::win32_file_openmode_single<om>::mode.dwCreationDisposition,
				details::dw_flag_attribute_with_perms(details::win32_file_openmode_single<om>::mode.dwFlagsAndAttributes,p)))
	{
		if constexpr ((om&open_mode::ate)!=open_mode::none)
			seek(*this,0,seekdir::end);
	}
	basic_win32_file(std::string_view filename,open_mode om,perms pm=static_cast<perms>(420)):basic_win32_io_handle<char_type>(nullptr)
	{
		auto const mode(details::calculate_win32_open_mode_with_perms(om,pm));
		if((om&open_mode::inherit)==open_mode::none)
		{
			native_handle()=details::create_file_a_impl
			(filename.data(),
			mode.dwDesiredAccess,
			mode.dwShareMode,
			mode.dwCreationDisposition,
			mode.dwFlagsAndAttributes);
		}
		else
		{
			native_handle()=details::create_file_a_impl<true>
				(filename.data(),
				mode.dwDesiredAccess,
				mode.dwShareMode,
				mode.dwCreationDisposition,
				mode.dwFlagsAndAttributes);
		}
		if ((om&open_mode::ate)!=open_mode::none)
			seek(*this,0,seekdir::end);
	}
	basic_win32_file(std::string_view file,std::string_view mode,perms pm=static_cast<perms>(420)):
		basic_win32_file(file,fast_io::from_c_mode(mode),pm){}

	~basic_win32_file()
	{
		this->close_impl();
	}
};

template<std::integral ch_type>
inline auto zero_copy_in_handle(basic_win32_io_handle<ch_type>& handle)
{
	return handle.native_handle();
}

template<std::integral ch_type>
inline void truncate(basic_win32_io_handle<ch_type>& handle,std::size_t size)
{
	seek(handle,size,seekdir::beg);
	if(!win32::SetEndOfFile(handle.native_handle()))
#ifdef __cpp_exceptions
		throw win32_error();
#else
		fast_terminate();
#endif
}

template<std::integral ch_type>
class basic_win32_pipe_unique:public basic_win32_io_handle<ch_type>
{
public:
	using char_type=ch_type;
	using basic_win32_io_handle<ch_type>::native_handle_type;
	using basic_win32_io_handle<ch_type>::native_handle;
	void close()
	{
		this->close_impl();
		native_handle() = nullptr;
	}
	basic_win32_pipe_unique()=default;
/*	win32_pipe_unique(win32_pipe_unique const&)=delete;
	win32_pipe_unique& operator=(win32_pipe_unique const&)=delete;
	win32_pipe_unique(win32_pipe_unique&& b) noexcept:win32_io_handle(b.native_handle())
	{
		b.native_handle()=nullptr;
	}
	win32_pipe_unique& operator=(win32_pipe_unique&& b) noexcept
	{
		if(std::addressof(b)!=this)
		{
			close_impl();
			native_handle() = b.native_handle();
			b.native_handle()=nullptr;
		}
		return *this;
	}*/
	~basic_win32_pipe_unique()
	{
		this->close_impl();
	}
};

template<std::integral ch_type>
class basic_win32_pipe
{
public:
	using char_type = ch_type;
	using native_handle_type = std::array<basic_win32_pipe_unique<ch_type>,2>;
private:
	native_handle_type pipes;
public:
	template<typename ...Args>
/*	requires requires(Args&& ...args)
	{
		{win32::CreatePipe(static_cast<void**>(static_cast<void*>(pipes.data())),static_cast<void**>(static_cast<void*>(pipes.data()+1)),std::forward<Args>(args)...)}->std::same_as<int>;
	}*/
	basic_win32_pipe(fast_io::native_interface_t, Args&& ...args)
	{
		if(!win32::CreatePipe(
			std::addressof(pipes.front().native_handle()),
			std::addressof(pipes.back().native_handle()),
			std::forward<Args>(args)...))
#ifdef __cpp_exceptions
			throw win32_error();
#else
			fast_terminate();
#endif
	}
	basic_win32_pipe()
	{
		win32::security_attributes sec_attr
		{
			.nLength=sizeof(win32::security_attributes),
			.bInheritHandle = true
		};
		
		if(!win32::CreatePipe(
			std::addressof(pipes.front().native_handle()),
			std::addressof(pipes.back().native_handle()),
			std::addressof(sec_attr),0))
#ifdef __cpp_exceptions
			throw win32_error();
#else
			fast_terminate();
#endif
	}
/*
	template<std::size_t om>
	basic_win32_pipe(open::interface_t<om>):basic_win32_pipe()
	{
		auto constexpr omb(om&~open::binary.value);
		static_assert(omb==open::in.value||omb==open::out.value||omb==(open::in.value|open::out.value),u8"pipe open mode must be in or out");
		if constexpr (!(om&~open::in.value)&&(om&~open::out.value))
			pipes.front().close();
		if constexpr ((om&~open::in.value)&&!(om&~open::out.value))
			pipes.back().close();
	}*/
	auto& native_handle()
	{
		return pipes;
	}
	auto& in()
	{
		return pipes.front();
	}
	auto& out()
	{
		return pipes.back();
	}
	void swap(basic_win32_pipe& o) noexcept
	{
		using std::swap;
		swap(pipes,o.pipes);
	}
};

template<std::integral ch_type,std::contiguous_iterator Iter>
inline Iter read(basic_win32_pipe<ch_type>& h,Iter begin,Iter end)
{
	return read(h.in(),begin,end);
}
template<std::integral ch_type,std::contiguous_iterator Iter>
inline auto write(basic_win32_pipe<ch_type>& h,Iter begin,Iter end)
{
	return write(h.out(),begin,end);
}

template<std::integral ch_type>
inline std::array<void*,2> redirect_handle(basic_win32_pipe<ch_type>& hd)
{
	return {hd.in().native_handle(),hd.out().native_handle()};
}


template<std::integral ch_type>
inline constexpr void flush(basic_win32_pipe<ch_type>&){}

using win32_io_handle=basic_win32_io_handle<char>;
using win32_file=basic_win32_file<char>;
using win32_pipe_unique=basic_win32_pipe_unique<char>;
using win32_pipe=basic_win32_pipe<char>;

using u8win32_io_handle=basic_win32_io_handle<char8_t>;
using u8win32_file=basic_win32_file<char8_t>;
using u8win32_pipe_unique=basic_win32_pipe_unique<char8_t>;
using u8win32_pipe=basic_win32_pipe<char8_t>;

inline constexpr std::uint32_t win32_stdin_number(-10);
inline constexpr std::uint32_t win32_stdout_number(-11);
inline constexpr std::uint32_t win32_stderr_number(-12);


inline win32_io_handle win32_stdin()
{
	return win32_stdin_number;
}

inline win32_io_handle win32_stdout()
{
	return win32_stdout_number;
}

inline win32_io_handle win32_stderr()
{
	return win32_stderr_number;
}

}
