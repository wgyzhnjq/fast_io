#pragma once

#include"fast_io_hosted.h"
//This header file defines in/out/err/log like traditional C/C++ does
//out and err will direct write to system call.
//The magic here is that data will first write to std::u8string and then write out
//There is no way to implement in without a buffer. input must have a buffer.

namespace fast_io
{
inline namespace
{

using in_type = fast_io::basic_iomutex<fast_io::basic_ibuf<native_io_handle>>;

in_type in(native_stdin_number);
inline native_io_handle out(native_stdout_number);
inline native_io_handle err(native_stderr_number);


inline auto out_buf()
{
	return basic_obuf<native_io_handle>(native_stdout_number);
}

inline auto log()
{
	return basic_obuf<native_io_handle>(native_stderr_number);
}

}
}