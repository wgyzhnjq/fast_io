#pragma once

//fast_io_devices.h defines commonly used io devices and their correlated mutex verisons.
#include"fast_io_hosted.h"


namespace fast_io
{
using pipe = io_wrapper<native_pipe>;

using inative_file = input_file_wrapper<native_file>;
using onative_file = output_file_wrapper<native_file>;
using ionative_file = io_file_wrapper<native_file>;

using sync = basic_sync<basic_file_wrapper<native_file,fast_io::open::app|fast_io::open::binary>,basic_ostring<std::string>>;
using fsync = basic_fsync<basic_file_wrapper<native_file,fast_io::open::app|fast_io::open::binary>,basic_ostring<std::string>>;

using omap = basic_omap<basic_file_wrapper<native_file,fast_io::open::trunc|fast_io::open::in|fast_io::open::out|fast_io::open::binary>,native_file_map>;

using sync_mutex = basic_iomutex<sync>;
using fsync_mutex = basic_iomutex<fsync>;

using ibuf = basic_ibuf<inative_file>;
using obuf = basic_obuf<onative_file>;
using iobuf = basic_iobuf<ionative_file>;

using ibuf_mutex = basic_iomutex<ibuf>;
using obuf_mutex = basic_iomutex<obuf>;
using iobuf_mutex = basic_iomutex<iobuf>;

using dynamic_buf = basic_iobuf<dynamic_stream>;


using u8pipe = io_wrapper<u8native_pipe>;

using u8inative_file = input_file_wrapper<u8native_file>;
using u8onative_file = output_file_wrapper<u8native_file>;
using u8ionative_file = io_file_wrapper<u8native_file>;

using u8sync = basic_sync<basic_file_wrapper<native_file,fast_io::open::app|fast_io::open::binary>,basic_ostring<std::u8string>>;
using u8fsync = basic_fsync<basic_file_wrapper<native_file,fast_io::open::app|fast_io::open::binary>,basic_ostring<std::u8string>>;

using u8sync_mutex = basic_iomutex<u8sync>;
using u8fsync_mutex = basic_iomutex<u8fsync>;

using u8ibuf = basic_ibuf<u8inative_file>;
using u8obuf = basic_obuf<u8onative_file>;
using u8iobuf = basic_iobuf<u8ionative_file>;

using u8ibuf_mutex = basic_iomutex<u8ibuf>;
using u8obuf_mutex = basic_iomutex<u8obuf>;
using u8iobuf_mutex = basic_iomutex<u8iobuf>;

using u8dynamic_buf = basic_iobuf<u8dynamic_stream>;

}