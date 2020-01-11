#pragma once

#include"fast_io_hosted.h"

namespace fast_io
{
inline namespace
{
using in_type = fast_io::basic_iomutex<fast_io::basic_ibuf<native_io_handle>>;

in_type in(native_stdin_number);
}
}