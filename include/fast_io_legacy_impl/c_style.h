#pragma once
#include<cstdio>
#include<cwchar>

namespace fast_io
{

namespace details
{
template<open_mode om>
struct c_open_mode
{
inline static constexpr std::string_view value=to_c_mode(om);
};
}
template<std::integral ch_type>
class basic_c_io_observer_unlocked
{
public:
	std::FILE *fp=nullptr;
	using char_type = ch_type;
	using native_handle_type = std::FILE*;
	constexpr auto& native_handle() noexcept
	{
		return fp;
	}
	constexpr auto& native_handle() const noexcept
	{
		return fp;
	}
	explicit operator basic_posix_io_observer<char_type>() const
	{

		auto fd(
#if defined(__WINNT__) || defined(_MSC_VER)
	_fileno(fp)
#else
	::fileno_unlocked(fp)
#endif
);
		if(fd<0)
#ifdef __cpp_exceptions
			throw std::system_error(errno,std::system_category());
#else
			fast_terminate();
#endif
		return static_cast<basic_posix_io_observer<char_type>>(fd);
	}
#if defined(__WINNT__) || defined(_MSC_VER)
	explicit operator basic_win32_io_observer<char_type>() const
	{
		return static_cast<basic_win32_io_observer<char_type>>(static_cast<basic_posix_io_observer<char_type>>(*this));
	}
#endif
	constexpr void reset() noexcept
	{
		fp=nullptr;
	}
};

template<std::integral ch_type>
inline auto redirect_handle(basic_c_io_observer_unlocked<ch_type>& h)
{
#if defined(__WINNT__) || defined(_MSC_VER)
	return static_cast<basic_win32_io_observer<ch_type>>(h).native_handle();
#else
	return static_cast<basic_posix_io_observer<ch_type>>(h).native_handle();
#endif
}

using c_io_observer_unlocked = basic_c_io_observer_unlocked<char>;

/*
#ifdef _MSC_VER

struct _iobuf
{
char *_ptr;
int _cnt;
char *_base;
int _flag;
int _file;
int _charbuf;
int _bufsiz;
char *_tmpfname;
};
#endif
*/
template<std::contiguous_iterator Iter>
inline Iter read(c_io_observer_unlocked& cfhd,Iter begin,Iter end)
{
	std::size_t const count(end-begin);
	std::size_t const r(
//mingw
#ifdef _MSC_VER
	_fread_nolock
#elif _POSIX_SOURCE
	fread_unlocked
#else
	fread
#endif
	(std::to_address(begin),sizeof(*begin),count,cfhd.native_handle()));
	if(r==count||std::feof(cfhd.native_handle()))
		return begin+r;
#ifdef __cpp_exceptions
	throw std::system_error(errno,std::generic_category());
#else
	fast_terminate();
#endif
}

template<std::contiguous_iterator Iter>
inline void write(c_io_observer_unlocked& cfhd,Iter begin,Iter end)
{
	std::size_t const count(end-begin);
	if(
#ifdef _MSC_VER
	_fwrite_nolock
#elif defined(_POSIX_SOURCE)
	fwrite_unlocked
#else
	fwrite
#endif
	(std::to_address(begin),sizeof(*begin),count,cfhd.native_handle())<count)
#ifdef __cpp_exceptions
		throw std::system_error(errno,std::system_category());
#else
		fast_terminate();
#endif
}
/*
template<bool err=false>
inline auto get(c_io_observer_unlocked& cfhd)
{
	auto ch(
#ifdef _MSC_VER
	_fgetc_nolock
#elif defined(_POSIX_SOURCE)
	fgetc_unlocked
#else
	fgetc
#endif
	(cfhd.native_handle()));
	if(ch==EOF)
	{
		if(
#ifdef _POSIX_SOURCE
	feof_unlocked
#else
	feof
#endif
(cfhd.native_handle()))
		{
			if constexpr(err)
				return std::pair<typename c_io_observer_unlocked::char_type,bool>{0,true};
			else
				throw eof();
		}
		throw std::system_error(errno,std::system_category());
	}
	if constexpr(err)
		return std::pair<typename c_io_observer_unlocked::char_type,bool>
		{
			std::char_traits<typename c_io_observer_unlocked::char_type>::to_char_type(ch),false
		};
	else
		return std::char_traits<typename c_io_observer_unlocked::char_type>::to_char_type(ch);
}
*/
inline void put(c_io_observer_unlocked& cfhd,typename c_io_observer_unlocked::char_type ch)
{
	if(
#if defined(_MSC_VER)
		_fputc_nolock
#elif defined(_POSIX_SOURCE)
		fputc_unlocked
#else
		fputc
#endif
	(static_cast<char>(ch),cfhd.native_handle())==EOF)

#ifdef __cpp_exceptions
		throw std::system_error(errno,std::system_category());
#else
		fast_terminate();
#endif
}

inline void flush(c_io_observer_unlocked& cfhd)
{
	if(
#if defined(_MSC_VER)
		_fflush_nolock
#elif defined(_POSIX_SOURCE)
		fflush_unlocked
#else
		fflush
#endif
	(cfhd.native_handle()))
#ifdef __cpp_exceptions
		throw std::system_error(errno,std::system_category());
#else
		fast_terminate();
#endif
}

template<typename T,std::integral U>
inline void seek(c_io_observer_unlocked& cfhd,seek_type_t<T>,U i,seekdir s=seekdir::beg)
{
	if(
#if defined(__WINNT__) || defined(_MSC_VER)
		_fseek_nolock
#elif defined(_POSIX_SOURCE)
		fseek_unlocked
#else
		fseek
#endif
	(cfhd.native_handle(),seek_precondition<long,T,char>(i),static_cast<int>(s)))
		throw std::system_error(errno,std::system_category()); 
}

template<std::integral U>
inline void seek(c_io_observer_unlocked& cfhd,U i,seekdir s=seekdir::beg)
{
	seek(cfhd,seek_type<char>,i,s);
}



//Exploiting the library internal implementation for performance since the stdio.h performance is SO TERRIBLE
#ifdef __MINGW32__
//cannot convert to char8_t due to strict aliasing rule violation
inline char* oreserve(c_io_observer_unlocked& cfhd,std::size_t n)
{
	auto& h(*cfhd.native_handle());
	if((h._cnt-=n)<=0) [[unlikely]]
	{
		h._cnt+=n;
		return nullptr;
	}
	return h._ptr+=n;
}

inline void orelease(c_io_observer_unlocked& cfhd,std::size_t n)
{
	auto& h(*(_iobuf*)cfhd.native_handle());
	h._cnt+=n;
	h._ptr-=n;
}
#elif defined(__GNU_LIBRARY__)
inline char* oreserve(c_io_observer_unlocked& cfhd,std::size_t n)
{
	auto& h(*cfhd.native_handle());
	if(h._IO_write_end<=h._IO_write_ptr+n)
		return nullptr;
	return h._IO_write_ptr+=n;
}

inline void orelease(c_io_observer_unlocked& cfhd,std::size_t n)
{
	cfhd.native_handle()->_IO_write_ptr-=n;
}
#endif

class c_io_lock_guard;

template<std::integral ch_type>
class basic_c_io_observer
{
public:
	std::FILE *fp=nullptr;
	using lock_guard_type = c_io_lock_guard;
	using char_type = ch_type;
	using native_handle_type = std::FILE*;
	constexpr auto& native_handle() const noexcept
	{
		return fp;
	}
	constexpr auto& native_handle() noexcept
	{
		return fp;
	}
	explicit operator basic_posix_io_observer<char_type>() const
	{
		auto fd(
#if defined(__WINNT__) || defined(_MSC_VER)
	_fileno(fp)
#else
	::fileno(fp)
#endif
);
		if(fd<0)
#ifdef __cpp_exceptions
			throw std::system_error(errno,std::system_category());
#else
			fast_terminate();
#endif
		return static_cast<basic_posix_io_observer<char_type>>(fd);
	}
#if defined(__WINNT__) || defined(_MSC_VER)
	explicit operator basic_win32_io_observer<char_type>() const
	{
		return static_cast<basic_win32_io_observer<char_type>>(static_cast<basic_posix_io_observer<char_type>>(*this));
	}
#endif
};

using c_io_observer=basic_c_io_observer<char>;

inline auto mutex(c_io_observer& h)
{
	return h.native_handle();
}

inline auto unlocked_handle(c_io_observer& h)
{
	return c_io_observer_unlocked(h.native_handle());
}

class c_io_lock_guard
{
	std::FILE* const fp;
public:
	c_io_lock_guard(std::FILE* f):fp(f)
	{
#if defined(__WINNT__) || defined(_MSC_VER)
		_lock_file(fp);
#else
		flockfile(fp);
#endif
	}
	c_io_lock_guard(c_io_lock_guard const&) = delete;
	c_io_lock_guard& operator=(c_io_lock_guard const&) = delete;
	~c_io_lock_guard()
	{
#if defined(__WINNT__) || defined(_MSC_VER)
		_unlock_file(fp);
#else
		funlockfile(fp);
#endif
	}
};


template<std::contiguous_iterator Iter>
inline Iter read(c_io_observer& cfhd,Iter begin,Iter end)
{
	std::size_t const count(end-begin);
	std::size_t const r(std::fread(std::to_address(begin),sizeof(*begin),count,cfhd.native_handle()));
	if(r==count||std::feof(cfhd.native_handle()))
		return begin+r;
#ifdef __cpp_exceptions
	throw std::system_error(errno,std::system_category());
#else
	fast_terminate();
#endif
}

template<std::contiguous_iterator Iter>
inline void write(c_io_observer& cfhd,Iter begin,Iter end)
{
	std::size_t const count(end-begin);
	if(std::fwrite(std::to_address(begin),sizeof(*begin),count,cfhd.native_handle())<count)
#ifdef __cpp_exceptions
		throw std::system_error(errno,std::system_category());
#else
		fast_terminate();
#endif
}


inline void put(c_io_observer& cfhd,typename c_io_observer::char_type ch)
{
	if(std::fputc(ch,cfhd.native_handle())==EOF)
#ifdef __cpp_exceptions
		throw std::system_error(errno,std::system_category());
#else
		fast_terminate();
#endif
}

inline void flush(c_io_observer& cfhd)
{
	if(std::fflush(cfhd.native_handle()))
#ifdef __cpp_exceptions
		throw std::system_error(errno,std::system_category());
#else
		fast_terminate();
#endif
}

template<typename T,std::integral U>
inline auto seek(c_io_observer& cfhd,seek_type_t<T>,U i,seekdir s=seekdir::beg)
{
	if(std::fseek(cfhd.native_handle(),seek_precondition<long,T,typename c_io_observer::char_type>(i),static_cast<int>(s)))
#ifdef __cpp_exceptions
		throw std::system_error(errno,std::system_category());
#else
		fast_terminate();
#endif
	auto val(std::ftell(cfhd.native_handle()));
	if(val<0)
#ifdef __cpp_exceptions
		throw std::system_error(errno,std::system_category());
#else
		fast_terminate();
#endif
	return val;
}

template<std::integral U>
inline auto seek(c_io_observer& cfhd,U i,seekdir s=seekdir::beg)
{
	return seek(cfhd,seek_type<typename c_io_observer::char_type>,i,s);
}

template<typename T,
	bool buffer=true,
	bool use_fwide=false>
requires (!use_fwide||std::same_as<typename T::native_handle,wchar_t>)
class basic_c_file:public T
{
	void close_impl() noexcept
	{
		if(native_handle())
			std::fclose(native_handle());
	}
public:
	using T::native_handle;
	using char_type=typename T::char_type;
	using native_handle_type=typename T::native_handle_type;
	basic_c_file()=default;
/*
	basic_c_file(std::string_view name,std::string_view mode):T(std::fopen(name.data(),mode.data()))
	{
		if(native_handle()==nullptr)
#ifdef __cpp_exceptions
			throw std::system_error(errno,std::system_category());
#else
			fast_terminate();
#endif
	}
	basic_c_file(std::string_view file,open_mode const& m):basic_c_file(file,c_style(m))
	{
		if(with_ate(m))
			seek(*this,0,seekdir::end);
	}*/
//fdopen interface

	basic_c_file(basic_posix_io_handle<typename T::char_type>&& posix_handle,std::string_view mode):T(
#if defined(__WINNT__) || defined(_MSC_VER)
			::_fdopen
#else
			::fdopen
#endif
(posix_handle.native_handle(),mode.data()))
	{
		if(native_handle()==nullptr)
#ifdef __cpp_exceptions
			throw std::system_error(errno,std::system_category());
#else
			fast_terminate();
#endif
		posix_handle.reset();

		if(native_handle()==nullptr)
#ifdef __cpp_exceptions
			throw std::system_error(errno,std::system_category());
#else
			fast_terminate();
#endif
		if constexpr(use_fwide)
		{
			if(fwide(native_handle(),1)<=0)
#ifdef __cpp_exceptions
			{
				std::fclose(native_handle());
				throw std::system_error(std::make_error_code(std::errc::io_error));
			}
#else
				fast_terminate();
#endif
		}
		if constexpr(!buffer)
			std::setbuf(native_handle(),nullptr);
	}

	basic_c_file(basic_posix_io_handle<typename T::char_type>&& posix_handle,open_mode om):
		basic_c_file(std::move(posix_handle),to_c_mode(om)){}
	template<open_mode om>
	basic_c_file(basic_posix_io_handle<typename T::char_type>&& posix_handle,open_interface_t<om>):
		basic_c_file(std::move(posix_handle),details::c_open_mode<om>::value){}

#if defined(__WINNT__) || defined(_MSC_VER)
//windows specific. open posix file from win32 io handle
	basic_c_file(basic_win32_io_handle<typename T::char_type>&& win32_handle,std::string_view mode):
		basic_c_file(basic_posix_file<typename T::char_type>(std::move(win32_handle),mode),mode)
	{
	}
	basic_c_file(basic_win32_io_handle<typename T::char_type>&& win32_handle,open_mode om):
		basic_c_file(basic_posix_file<typename T::char_type>(std::move(win32_handle),om),to_c_mode(om))
	{
	}
	template<open_mode om>
	basic_c_file(basic_win32_io_handle<typename T::char_type>&& win32_handle,open_interface_t<om>):
		basic_c_file(basic_posix_file<typename T::char_type>(std::move(win32_handle),open_interface<om>),
			details::c_open_mode<om>::value)//open::c_style_interface_t<om>::mode)
	{
	}
#endif

	template<open_mode om,typename... Args>
	basic_c_file(std::string_view file,open_interface_t<om>,Args&& ...args):
		basic_c_file(basic_posix_file<typename T::char_type>(file,open_interface<om>,std::forward<Args>(args)...),
			open_interface<om>)
	{}
	template<typename... Args>
	basic_c_file(std::string_view file,open_mode om,Args&& ...args):
		basic_c_file(basic_posix_file<typename T::char_type>(file,om,std::forward<Args>(args)...),om)
	{}
	template<typename... Args>
	basic_c_file(std::string_view file,std::string_view mode,Args&& ...args):
		basic_c_file(basic_posix_file<typename T::char_type>(file,mode,std::forward<Args>(args)...),mode)
	{}

	basic_c_file(basic_c_file const&)=delete;
	basic_c_file& operator=(basic_c_file const&)=delete;
	basic_c_file(basic_c_file&& b) noexcept : T(b.native_handle())
	{
		b.native_handle() = nullptr;
	}
	basic_c_file& operator=(basic_c_file&& b) noexcept
	{
		if(std::addressof(b)!=this)
		{
			close_impl();
			native_handle()=b.native_handle();
			b.native_handle() = nullptr;
		}
		return *this;
	}
	~basic_c_file()
	{
		close_impl();
	}
};
template<std::integral ch_type>
inline auto redirect_handle(basic_c_io_observer<ch_type>& h)
{
#if defined(__WINNT__) || defined(_MSC_VER)
	return static_cast<basic_win32_io_observer<ch_type>>(h).native_handle();
#else
	return static_cast<basic_posix_io_observer<ch_type>>(h).native_handle();
#endif
}
using c_file = basic_c_file<c_io_observer>;
using c_file_unlocked = basic_c_file<c_io_observer_unlocked>;

#ifdef __linux__
template<std::integral ch_type>
inline auto zero_copy_in_handle(basic_c_io_observer_unlocked<ch_type>& h)
{
	return ::fileno_unlocked(h.native_handle());
}
template<std::integral ch_type>
inline auto zero_copy_out_handle(basic_c_io_observer_unlocked<ch_type>& h)
{
	return ::fileno_unlocked(h.native_handle());
}
#endif

}
