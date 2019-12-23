#ifndef TIMER_H
#define TIMER_H

#include<chrono>
#include"../../include/fast_io.h"
#include<string>

namespace cqw
{

class timer
{
	std::u8string_view s;
	std::chrono::high_resolution_clock::time_point t0;
public:
	template<typename ...Args>
	requires std::constructible_from<std::u8string_view,Args...>
	timer(Args&& ...args):s(std::forward<Args>(args)...),t0(std::chrono::high_resolution_clock::now()){}
	timer(const timer&) = delete;
	timer& operator=(const timer&) = delete;
	~timer()
	{
		try
		{
			println(fast_io::err,s,u8":\t",std::chrono::high_resolution_clock::now()-t0,fast_io::char_view(u8's'));
		}
		catch(...)
		{

		}
	}
};

}

#endif