#pragma once
#include<chrono>

namespace fast_io
{

//We use seconds since seconds is the standard unit of SI
//Use my own tweaked ryu algorithm for counting seconds
template<output_stream output,typename Rep,typename Period>
inline constexpr void print_define(output& out, std::chrono::duration<Rep,Period> const& duration)
{
	print(out,std::chrono::duration_cast<std::chrono::duration<double>>(duration).count(),fast_io::char_view(u8's'));
}

//2019✝️12🌙30☀️ ⛪6 19:10:03

template<character_output_stream output>
inline constexpr void print_define(output& out,std::tm t)
//due to std::tm is often not thread safe. We make a copy
{
	print(out,1900+t.tm_year,u8"✝️");
	if(t.tm_mon<9)
		put(out,u8'0');
	print(out,1+t.tm_mon,u8"🌙");
	if(t.tm_mday<9)
		put(out,u8'0');
	print(out,1+t.tm_mday,u8"☀️ ⛪",t.tm_wday);
	put(out,u8' ');
	if(t.tm_hour<10)
		put(out,u8'0');
	print(out,t.tm_hour);
	put(out,u8':');
	if(t.tm_min<10)
		put(out,u8'0');
	print(out,t.tm_min);
	put(out,u8':');
	if(t.tm_sec<10)
		put(out,u8'0');
	print(out,t.tm_sec);
}

template<character_output_stream output,typename Clock,typename Duration>
inline constexpr void print_define(output& out, std::chrono::time_point<Clock,Duration> const& tmp)
{
	auto tmt(Clock::to_time_t(tmp));
	print_define(out,*localtime(std::addressof(tmt)));
}

template<character_output_stream output,typename Clock,typename Duration>
inline constexpr void print_define(output& out, manip::utc<std::chrono::time_point<Clock,Duration> const> tmp)
{
	auto tmt(Clock::to_time_t(tmp.reference));
	print_define(out,*gmtime(std::addressof(tmt)));
}

template<character_output_stream output,typename Clock,typename Duration>
inline constexpr void print_define(output& out, manip::unix<std::chrono::time_point<Clock,Duration> const> tmp)
{
	print_define(out,std::chrono::duration_cast<std::chrono::duration<double>>(tmp.reference.time_since_epoch()));
}
/*
//current libraries have no these facilities
template<output_stream output>
inline constexpr void print_define(output& out, std::chrono::time_zone const& tmp)
{
	print(out,tmp.name());
}
*/

}