#pragma once

extern "C" char **environ;
namespace fast_io
{

class posix_process
{
	pid_t pid;
public:
	using native_handle_t = pid_t;
	posix_process():pid(
#if defined(__linux__)&&defined(__x86_64__)
	system_call<57,pid_t>(
#else
	fork()
#endif
)
	{
		system_call_throw_error(pid);
	}
	inline auto id()
	{
		return pid;
	}
};

inline bool is_child(posix_process const& p)
{
	return !p.id();
}
inline bool is_parent(posix_process const& p)
{
	return p.id();
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
	details::execve_impl(path.data(),arr.data(),environ);
}

namespace details
{

template<std::contiguous_iterator>
inline void deal_with_paramters(Iter begin,Iter end,std::ranges::contiguous_range auto& paramaters)
{
	auto ptr(temp.data());
	for(;begin!=end;++begin)
	{
		*ptr=begin->data();
		++ptr;
	}
	*ptr={};
}

inline void posix_exec_impl(std::contiguous_range auto& temp,
	std::string& path,
	std::ranges::contiguous_range auto& args)
{
	temp.front()=path.data();
	deal_with_paramters(temp.begin()+1,temp.end(),args);
	execve_impl(path.data(),temp.data(),environ);
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
inline void wait(posix_process const& p)
{
	system_call_throw_error(
#if defined(__linux__)&&defined(__x86_64__)
	system_call<61,int>(p.id(),nullptr,0,nullptr)
#else
	waitpid(p.id(),nullptr,0)
#endif
	);
}
}