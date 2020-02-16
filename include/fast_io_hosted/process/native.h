#pragma once

#if defined(__WINNT__) || defined(_MSC_VER)
#include"nt_error.h"
#include"nt_fork.h"
#include"win32.h"
#include<process.h>
#else
#include"posix.h"
#endif
#include"posix_exec.h"

namespace fast_io
{
#if defined(__WINNT__) || defined(_MSC_VER)
using process = win32_process;
using jprocess = win32_jprocess;
#else
using process = posix_process;
using jprocess = posix_jprocess;
#endif
}