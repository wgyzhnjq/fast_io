#pragma once


namespace fast_io
{
namespace details
{
extern "C" char **environ;
}
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
};

inline bool is_child(posix_process const& p)
{
	return !p.native_handle();
}
inline bool is_parent(posix_process const& p)
{
	return p.native_handle();
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
	posix_waiting_process():pid(
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
	posix_waiting_process(posix_waiting_process const&)=delete;
	posix_waiting_process& operator=(posix_waiting_process const&)=delete;
	posix_waiting_process(posix_waiting_process&& bmv) noexcept:pid(bmv.pid)
	{
		bmv.pid=-1;
	}
	posix_waiting_process& operator=(posix_waiting_process&& bmv) noexcept
	{
		if(pid!=bmv.pid)
		{
			close_impl();
			pid=bmv.pid;
			bmv.pid=-1;
		}
		return *this;
	}
	~posix_waiting_process()
	{
		close_impl();
	}
};
inline bool is_child(posix_waiting_process const& p)
{
	return !p.native_handle();
}
inline bool is_parent(posix_waiting_process const& p)
{
	return 0<p.native_handle();
}
inline bool has_detached(posix_waiting_process const& p)
{
	return p.native_handle()<0;
}

namespace details
{
template<typename... Args>
inline void execve_impl(Args&& ...args)
{
	system_call_throw_error(
#if defined(__linux__)&&defined(__x86_64__)
		system_call<59,int>
#else
		::execve
#endif
	(std::forward<Args>(args)...));
}
}

inline void posix_exec(std::string path)
{
	std::array<char*,2> arr{path.data()};
	details::execve_impl(path.data(),arr.data(),details::environ);
}

namespace details
{

template<std::contiguous_iterator Iter>
inline void deal_with_paramters(
	Iter begin,
	Iter end,
	std::contiguous_iterator auto output)
{
	for(;begin!=end;++begin)
	{
		*output=begin->data();
		++output;
	}
	*output={};
}

inline void posix_exec_impl(std::ranges::contiguous_range auto& temp,
	std::string& path,
	std::ranges::contiguous_range auto& args)
{
	temp.front()=path.data();
	deal_with_paramters(
		args.begin(),
		args.end(),
		temp.data()+1);
	execve_impl(path.data(),temp.data(),details::environ);
}
}

inline void posix_exec(std::string path,std::vector<std::string> args)
{
	if(args.size()<126)[[likely]]
	{
		std::array<char*,128> temp;
		details::posix_exec_impl(temp,path,args);
	}
	else
	{
		std::vector<char*> temp(args.size()+1);
		details::posix_exec_impl(temp,path,args);
	}
}

template<std::size_t n>
inline void posix_exec(std::string path,std::array<std::string,n> args)
{
	if constexpr(n<126)
	{
		std::array<char*,n+2> temp;
		details::posix_exec_impl(temp,path,args);
	}
	else
	{
		std::vector<char*> temp(n+1);
		details::posix_exec_impl(temp,path,args);
	}
}

/*
inline void posix_exec(std::string_view path,std::ranges::sized_range auto& parameters,std::ranges::sized_range auto& environment)
{
	if(std::ranges::size(parameters)<127)[[likely]]
	{
		std::array<char*,128> temp;
		if(std::ranges::size(parameters)<127)
		{
			std::array<char*,128> env_temp;
			details::posix_exec_impl(temp,path,parameters);
		}
	}
	else
	{
		std::vector<char*> temp(std::ranges::size(parameters)+1);
		details::posix_exec_impl(temp,path,parameters);
	}
}
*/
}