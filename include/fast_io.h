#pragma once

#include"fast_io_hosted.h"
//This header file defines in/out/err/log like traditional C/C++ does
//out and err will direct write to system call.
//The magic here is that data will first write to std::string and then write out
//There is no way to implement in without a buffer. input must have a buffer.

namespace fast_io
{
inline namespace
{

using in_type = fast_io::basic_iomutex<istack<system_io_handle,4096>>;

inline in_type in(native_stdin_number);
inline system_io_handle out(native_stdout_number);
inline system_io_handle err(native_stderr_number);


inline auto out_buf()
{
	return basic_obuf<system_io_handle>(native_stdout_number);
}

inline auto log()
{
	return basic_obuf<system_io_handle>(native_stderr_number);
}

}
}