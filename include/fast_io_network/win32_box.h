#pragma once
#include"win32_library.h"

namespace fast_io
{

template<std::integral ch_type>
requires (std::same_as<ch_type,char>||std::same_as<ch_type,wchar_t>)
class basic_win32_box
{
public:
	using char_type = ch_type;
	using string_view_type = std::basic_string_view<char_type>;
private:
	std::basic_string_view<char_type> view;
public:
	constexpr basic_win32_box(std::basic_string_view<char_type> view):view(view.data()){}
	constexpr std::basic_string_view<char_type>& title()
	{
		return view;
	}
};

template<std::integral ch_type,std::contiguous_iterator Iter>
requires (std::same_as<ch_type,typename std::iterator_traits<Iter>::value_type>)
inline void write(basic_win32_box<ch_type>& box,Iter a,Iter b)
{
	fast_io::details::win32_library const user32_dll(L"user32.dll");
	if constexpr(std::same_as<ch_type,char>)
	{
		auto MessageBoxA(bit_cast<int(*)(void*,char const*,char const*,std::uint32_t)>
			(::GetProcAddress(user32_dll.get(),"MessageBoxA")));
		if(MessageBoxA==nullptr)
#ifdef __cpp_exceptions
			throw win32_error();
#else
			fast_terminate();
#endif
		std::string str(a,b);
		if(!MessageBoxA(nullptr,str.c_str(),box.title().data(),0x00000040L/*MB_ICONINFORMATION*/))
#ifdef __cpp_exceptions
			throw win32_error();
#else
			fast_terminate();
#endif
	}
	else
	{
		auto MessageBoxW(bit_cast<int(*)(void*,wchar_t const*,wchar_t const*,std::uint32_t)>
			(::GetProcAddress(user32_dll.get(),"MessageBoxW")));
		if(MessageBoxW==nullptr)
#ifdef __cpp_exceptions
			throw win32_error();
#else
			fast_terminate();
#endif
		std::wstring str(a,b);
		if(!MessageBoxW(nullptr,str.c_str(),box.title().data(),0x00000040L))
#ifdef __cpp_exceptions
			throw win32_error();
#else
			fast_terminate();
#endif
	}
}

template<std::integral ch_type>
inline constexpr void flush(basic_win32_box<ch_type>&){}

template<std::integral ch_type>
using basic_native_box = basic_win32_box<ch_type>;

using win32_box = basic_native_box<char>;
using wwin32_box = basic_native_box<wchar_t>;

using native_box = win32_box;
using wnative_box = wwin32_box;

}