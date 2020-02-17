#pragma once

namespace fast_io
{
class posix_process
{
	pid_t pid;
public:
	using native_handle_t = pid_t;
	posix_process():pid(
#if defined(__linux__)&&defined(__x86_64__)
	system_call<57,pid_t>
#else
	fork
#endif
())
	{
		system_call_throw_error(pid);
	}
	inline auto native_handle() const
	{
		return pid;
	}
	inline void detach()
	{
		pid=-1;
	}
	void join()
	{
		system_call_throw_error(
	#if defined(__linux__)&&defined(__x86_64__)
		system_call<61,int>(pid,nullptr,0,nullptr)
	#else
		waitpid(pid,nullptr,0)
	#endif
		);
	}
	auto id() const
	{
		return pid;
	}
};

inline bool is_child(posix_process const& p)
{
	return !p.native_handle();
}
inline bool is_parent(posix_process const& p)
{
	return 0<p.native_handle();
}

inline bool has_detached(posix_process const& p)
{
	return p.native_handle()<0;
}

class posix_jprocess
{
	pid_t pid;
	void close_impl()
	{
		if(0<pid)
		{
	#if defined(__linux__)&&defined(__x86_64__)
		system_call<61,int>(pid,nullptr,0,nullptr);
	#else
		waitpid(pid,nullptr,0);
	#endif
		}
	}
public:
	using native_handle_t = pid_t;
	posix_jprocess():pid(
#if defined(__linux__)&&defined(__x86_64__)
	system_call<57,pid_t>
#else
	fork
#endif
())
	{
		system_call_throw_error(pid);
	}
	inline auto native_handle() const
	{
		return pid;
	}
	inline void join()
	{
		system_call_throw_error(
	#if defined(__linux__)&&defined(__x86_64__)
		system_call<61,int>(pid,nullptr,0,nullptr)
	#else
		waitpid(pid,nullptr,0)
	#endif
		);
	}
	inline void detach()
	{
		pid=-1;
	}
	posix_jprocess(posix_jprocess const&)=delete;
	posix_jprocess& operator=(posix_jprocess const&)=delete;
	posix_jprocess(posix_jprocess&& bmv) noexcept:pid(bmv.pid)
	{
		bmv.pid=-1;
	}
	posix_jprocess& operator=(posix_jprocess&& bmv) noexcept
	{
		if(pid!=bmv.pid)
		{
			close_impl();
			pid=bmv.pid;
			bmv.pid=-1;
		}
		return *this;
	}
	auto id() const
	{
		return pid;
	}
	~posix_jprocess()
	{
		close_impl();
	}
};
inline bool is_child(posix_jprocess const& p)
{
	return !p.native_handle();
}
inline bool is_parent(posix_jprocess const& p)
{
	return 0<p.native_handle();
}
inline bool has_detached(posix_jprocess const& p)
{
	return p.native_handle()<0;
}

}