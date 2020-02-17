#pragma once

#if defined(__WINNT__) || defined(_MSC_VER)
//#include"nt_error.h"
//#include"nt_fork.h"
#include<process.h>
#include"win32.h"
#else
#include"posix_exec.h"
#include"posix.h"
#endif

//#include"process.h"

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