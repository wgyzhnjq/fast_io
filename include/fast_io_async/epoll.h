#pragma once

#include<sys/epoll.h>

namespace fast_io
{

namespace epoll
{

struct close_on_exec_function_invoked_t
{
explicit constexpr close_on_exec_function_invoked_t()=default;
};

inline constexpr close_on_exec_function_invoked_t close_on_exec_function_invoked{};


class handle_pool
{
	int fd;
	void close_impl() noexcept
	{
		if(fd!=-1)
			close(fd);
	}
public:
	using native_handle_type = int;
	explicit handle_pool(std::integral auto counts)
	{
		if(::epoll_create(static_cast<int>(counts))==-1)
			throw std::system_error(errno,std::generic_category());
	}
	explicit handle_pool(std::integral auto counts,close_on_exec_function_invoked_t)
	{
		if(::epoll_create1(static_cast<int>(counts)|EPOLL_CLOEXEC)==-1)
			throw std::system_error(errno,std::generic_category());
	}
	handle_pool(handle_pool const& dp):fd(dup(dp.fd))
	{
		if(fd<0)
			throw std::system_error(errno,std::generic_category());
	}
	handle_pool& operator=(handle_pool const& dp)
	{
		auto newfd(dup2(dp.fd,fd));
		if(newfd<0)
			throw std::system_error(errno,std::generic_category());
		fd=newfd;
		return *this;
	}
	handle_pool(handle_pool&& b) noexcept : handle_pool(b.fd)
	{
		b.fd = -1;
	}
	handle_pool& operator=(handle_pool&& b) noexcept
	{
		if(std::addressof(b)!=this)
		{
			close_impl();
			fd=b.fd;
			b.fd = -1;
		}
		return *this;
	}
	auto native_handle() const
	{
		return fd;
	}
	~handle_pool()
	{
		if(fd!=-1)
			close(fd);
	}
};

//TODO: give them more meaningful names
enum class event:std::uint32_t
{
none=0,in=1,pri=2,out=4,err=8,hup=0x10,nval=0x20,rdnorm=0x40,
rdband=0x80,wrnorm=0x100,wrband=0x200,message=0x400,
rdhup=0x2000,exclusive=1u<<28,oneshot=1u<<30,edge_trigger=1u<<31
};

constexpr event operator&(event x, event y) noexcept
{
using utype = typename std::underlying_type<event>::type;
return static_cast<event>(static_cast<utype>(x) & static_cast<utype>(y));
}

constexpr event operator|(event x, event y) noexcept
{
using utype = typename std::underlying_type<event>::type;
return static_cast<event>(static_cast<utype>(x) | static_cast<utype>(y));
}

constexpr event operator^(event x, event y) noexcept
{
using utype = typename std::underlying_type<event>::type;
return static_cast<event>(static_cast<utype>(x) ^ static_cast<utype>(y));
}

constexpr event operator~(event x) noexcept
{
using utype = typename std::underlying_type<event>::type;
return static_cast<event>(~static_cast<utype>(x));
}

inline event& operator&=(event& x, event y) noexcept{return x=x&y;}

inline event& operator|=(event& x, event y) noexcept{return x=x|y;}

inline event& operator^=(event& x, event y) noexcept{return x=x^y;}

template<typename T>
concept epoll_stream = requires(T& t)
{
	{ultimate_native_handle(out)}->std::same_as<int>;
};

template<epoll_stream output>
inline void add_control(handle_pool& pool,output& out,event e,std::integral auto value)
{
	epoll_event evt{static_cast<std::uint32_t>(e),{.u64=static_cast<std::uint64_t>(value)}};
	if(::epoll_ctl(pool.native_handle(),1,ultimate_native_handle(out),std::addressof(evt))==-1)
		throw std::system_error(errno,std::generic_category());
}

template<epoll_stream output>
inline void delete_control(handle_pool& pool,output& out,std::integral auto value)
{
	epoll_event evt{1,{.u64=static_cast<std::uint64_t>(value)}};
	if(::epoll_ctl(pool.native_handle(),2,ultimate_native_handle(out),std::addressof(evt))==-1)
		throw std::system_error(errno,std::generic_category());
}
template<epoll_stream output>
inline void modify_control(handle_pool& pool,output& out,event e,std::integral auto value)
{
	epoll_event evt{static_cast<std::uint32_t>(e),{.u64=static_cast<std::uint64_t>(value)}};
	if(::epoll_ctl(pool.native_handle(),3,ultimate_native_handle(out),std::addressof(evt))==-1)
		throw std::system_error(errno,std::generic_category());
}

template<typename Rep,typename Period>
[[nodiscard]] inline std::size_t wait(handle_pool& pool,event e,std::integral auto value,std::integral auto max_events,std::chrono::duration<Rep,Period> const& timeout)
{
	epoll_event evt{static_cast<std::uint32_t>(e),{.u64=static_cast<std::uint64_t>(value)}};
	int ret(::epoll_wait(pool.native_handle(),std::addressof(evt),static_cast<int>(max_events),
		static_cast<int>(std::chrono::duration_cast<std::chrono::milliseconds>(timeout).count())==1));
	if(ret==-1)
		throw std::system_error(errno,std::generic_category());
	return ret;
}

[[nodiscard]] inline std::size_t wait(handle_pool& pool,event e,std::integral auto value,std::integral auto max_events)
{
	epoll_event evt{static_cast<std::uint32_t>(e),{.u64=static_cast<std::uint64_t>(value)}};
	int ret(::epoll_wait(pool.native_handle(),std::addressof(evt),static_cast<int>(max_events),-1));
	if(ret==-1)
		throw std::system_error(errno,std::generic_category());
	return ret;
}


/*
template<output_stream output>
inline void delete_control(handle_pool& pool,output& out,std::integral auto value)
{
	if(::epoll_ctl(pool.native_handle(),std::addressof(func),2,nullptr,epoll_event{0,static_cast<std::uint64_t>(value)})==-1)
		throw std::system_error(errno,std::generic_category());
}

template<output_stream output>
inline void modify_control(handle_pool& pool,output& out,event e,std::integral auto value)
{
	if(::epoll_ctl(pool.native_handle(),std::addressof(func),3,epoll_event{e,static_cast<std::uint64_t>(value)})==-1)
		throw std::system_error(errno,std::generic_category());
}
*/
}

}