#pragma once

#if defined(__WINNT__) || defined(_MSC_VER)
#include"fast_io_hosted.h"
#include"fast_io_network/win32_box.h"

namespace fast_io
{

using namespace std::string_view_literals;
inline
#if __cpp_constinit >= 201907L
constinit
#endif
fast_io::native_box box("fast io"sv);
inline
#if __cpp_constinit >= 201907L
constinit
#endif
fast_io::wnative_box wbox(L"fast io"sv);


#ifndef NDEBUG

inline
#if __cpp_constinit >= 201907L
constinit
#endif
fast_io::native_box debug_box("fast io debugging"sv);
inline
#if __cpp_constinit >= 201907L
constinit
#endif
fast_io::wnative_box debug_wbox(L"fast io debugging"sv);

#endif

}

#endif