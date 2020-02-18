#pragma once

namespace fast_io
{

/*
https://www.youtube.com/watch?v=c1gO9aB9nbs&t=3166s
CppCon 2014: Herb Sutter "Lock-Free Programming (or, Juggling Razor Blades), Part I"
*/
template<std::movable acceptor_type,std::size_t size=900,typename server_type,typename Func>
inline void thread_pool_accept_callback(server_type& server,Func&& func)
{
	std::vector<std::thread> pool;
	pool.reserve(size);
	std::mutex mtx;
	std::condition_variable cv;
	std::vector<std::optional<acceptor_type>> slot(size);
	for(std::size_t i{};i!=size;++i)
		pool.emplace_back([func,&mtx,&cv,&slot](std::size_t i)
		{
			for(std::optional<acceptor_type>& opt(slot[i]);;)
			{
				std::unique_lock ul{mtx};
				cv.wait(ul,[&opt](){return opt.has_value();});
				auto acc(*std::move(opt));
				opt.reset();
				func(acc);
			}
		},i);
	for(auto i(slot.begin());;)
	{
		std::optional<acceptor_type> opt(std::in_place,server);
		for(auto e(slot.end());*i;)
			if(++i==e)
				i=slot.begin();
		*i=std::move(opt);
		cv.notify_all();
	}
}

#if __cpp_coroutines >= 201902L
struct task
{
struct promise_type
{
auto get_return_object() { return task{}; }
auto initial_suspend() { return std::suspend_never{}; }
auto final_suspend() { return std::suspend_never{}; }
void unhandled_exception() { std::terminate(); }
void return_void() {}
};
};

template<typename server_type,typename acceptor_type>
class basic_thread_pool_awaitable
{
	server_type* pserver;
	acceptor_type* pacceptor;
public:
	constexpr basic_thread_pool_awaitable(server_type& serv):pserver(std::addressof(serv)){}
	bool await_ready() const { return false; }
	acceptor_type& await_resume() { return *pacceptor; }
	void await_suspend(std::coroutine_handle<> handle)
	{
		auto f([handle, this](acceptor_type& acc)
		{
			pacceptor = std::addressof(acc);
			handle.resume();
		});
		thread_pool_accept_callback<acceptor_type>(*pserver,f);
	}
};
#endif
}