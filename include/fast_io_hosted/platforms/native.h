#pragma once
#if defined(__WINNT__) || defined(_MSC_VER)
#include"win32.h"
#include"win32_mapping.h"
#endif

#include"posix.h"
#ifdef _POSIX_C_SOURCE
#include"posix_mapping.h"
#endif

namespace fast_io
{

#if defined(__WINNT__) || defined(_MSC_VER)
inline constexpr std::uint32_t native_stdin_number(win32_stdin_number);
inline constexpr std::uint32_t native_stdout_number(win32_stdout_number);
inline constexpr std::uint32_t native_stderr_number(win32_stderr_number);
using native_file = win32_file;
using native_io_handle = win32_io_handle;
using native_pipe_unique = win32_pipe_unique;
using native_pipe = win32_pipe;
using u8native_file = u8win32_file;
using u8native_io_handle = u8win32_io_handle;
using u8native_pipe_unique = u8win32_pipe_unique;
using u8native_pipe = u8win32_pipe;
using native_file_map = win32_file_map;

#else
inline constexpr std::uint32_t native_stdin_number(posix_stdin_number);
inline constexpr std::uint32_t native_stdout_number(posix_stdout_number);
inline constexpr std::uint32_t native_stderr_number(posix_stderr_number);

using native_file = posix_file;
using native_io_handle = posix_io_handle;
using native_pipe_unique = posix_pipe_unique;
using native_pipe = posix_pipe;
using u8native_file = u8posix_file;
using u8native_io_handle = u8posix_io_handle;
using u8native_pipe_unique = u8posix_pipe_unique;
using u8native_pipe = u8posix_pipe;
using native_file_map = posix_file_map;
#endif


inline native_io_handle native_stdin()
{
	return native_stdin_number;
}
inline native_io_handle native_stdout()
{
	return native_stdout_number;
} 
inline native_io_handle native_stderr()
{
	return native_stderr_number;
}

}