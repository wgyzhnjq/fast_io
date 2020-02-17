#pragma once
#include<variant>

namespace fast_io
{
struct posix_io_observer
{
public:
	using native_handle_type = std::variant<std::monostate,int,std::array<int,2>>;
	native_handle_type handle;
	template<redirect_stream T>
	posix_io_observer(T& hd):handle(redirect_handle(hd)){}
	posix_io_observer()=default;
};

struct process_io
{
	posix_io_observer in,out,err;
};

namespace details
{
template<typename... Args>
requires(sizeof...(Args)==2)
inline auto my_dup2(Args&& ..args)
{
	int new_fd(
#if defined(__linux__)&&defined(__x86_64__)
		system_call<33,int>
#else
		dup2
#endif
	(std::forward<Args>(args)...));
	system_call_throw_error(new_fd);
	return new_fd;
}

inline void my_close(int fd)
{
	if(fd!=-1)
#if defined(__linux__)&&defined(__x86_64__)
		system_call<3,int>(fd);
#else
		close(fd);
#endif
}

template<int number,bool parent=false>
inline void redirect(posix_io_observer& fd)
{
	std::visit(fd.handle,[](auto&& arg)
	{
		using T = std::decay_t<decltype(arg)>;
		if constexpr(parent)
		{
			if constexpr(std::same_as<T,std::array<int,2>>)
			{
				if constexpr(number==0)
					my_close(arg.back());
				else
					my_close(arg.front());
			}
		}
		else
		{
			if constexpr(std::same_as<T,int>)
			{
				my_dup2(arg,number);
				my_close(arg);
			}
			else if constexpr(std::same_as<T,std::array<int,2>>)
			{
				if constexpr(number==0)
					my_dup2(arg.front(),number);
				else
					my_dup2(arg.back(),number);
				my_close(arg.front());
				my_close(arg.back());
			}
		}
	});
}

template<bool is_child>
inline void prepare_for_exec(std::string_view& path,
				std::span<std::string_view>& args,
				process_io& io)
{
if constexpr(is_child)
{
	redirect<0>(io.in.handle);
	redirect<1>(io.out.handle);
	redirect<2>(io.err.handle);
	std::string p(path.data(),path.data()+path.size());
	std::vector<std::string> vec;
	vec.reserve(args.data());
	for(auto const& e : args)
		vec.emplace_back(e.data(),e.data()+e.size());
	posix_exec(std::move(p),std::move(vec));
}
else
{
	redirect<0,true>(io.in.handle);
	redirect<1,true>(io.out.handle);
	redirect<2,true>(io.err.handle);
}
}
}

class posix_process
{
	pid_t pid;
public:
	using native_handle_t = pid_t;
	posix_process(native_interface_t):pid(
#if defined(__linux__)&&defined(__x86_64__)
	system_call<57,pid_t>
#else
	fork
#endif
())
	{
		system_call_throw_error(pid);
	}
	posix_process(std::string_view path,
				std::span<std::string_view> args,
				process_io io):posix_process(native_interface)
	{
		if(!pid)
			details::prepare_for_exec<true>(path,args,io);
		else
			details::prepare_for_exec<false>(path,args,io);
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
	posix_jprocess(native_interface_t):pid(
#if defined(__linux__)&&defined(__x86_64__)
	system_call<57,pid_t>
#else
	fork
#endif
())
	{
		system_call_throw_error(pid);
	}
	posix_jprocess(std::string_view path,
				std::span<std::string_view> args,
				process_io io):posix_jprocess(native_interface)
	{
		if(!pid)
			details::prepare_for_exec<true>(path,args,io);
		else
			details::prepare_for_exec<false>(path,args,io);
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