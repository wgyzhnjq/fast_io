#pragma once

//fast_io_devices.h defines commonly used io devices and their correlated mutex verisons.
#include"fast_io_hosted.h"


namespace fast_io
{
using pipe = io_wrapper<native_pipe>;

using inative_file = input_file_wrapper<native_file>;
using onative_file = output_file_wrapper<native_file>;
using ionative_file = io_file_wrapper<native_file>;

using sync_file = basic_sync<basic_file_wrapper<native_file,fast_io::open::app|fast_io::open::binary>,basic_ostring<std::string>>;
using fsync_file = basic_fsync<basic_file_wrapper<native_file,fast_io::open::app|fast_io::open::binary>,basic_ostring<std::string>>;

using omap_file = basic_omap<basic_file_wrapper<native_file,fast_io::open::trunc|fast_io::open::in|fast_io::open::out|fast_io::open::binary>,native_file_map>;

using sync_file_mutex = basic_iomutex<sync_file>;
using fsync_file_mutex = basic_iomutex<fsync_file>;

using ibuf_file = basic_ibuf<inative_file>;
using obuf_file = basic_obuf<onative_file>;
using iobuf_file = basic_iobuf<ionative_file>;

using ibuf_file_mutex = basic_iomutex<ibuf_file>;
using obuf_file_mutex = basic_iomutex<obuf_file>;
using iobuf_file_mutex = basic_iomutex<iobuf_file>;

using dynamic_buf = basic_iobuf<dynamic_stream>;


using u8pipe = io_wrapper<u8native_pipe>;

using u8inative_file = input_file_wrapper<u8native_file>;
using u8onative_file = output_file_wrapper<u8native_file>;
using u8ionative_file = io_file_wrapper<u8native_file>;

using u8sync_file = basic_sync<basic_file_wrapper<native_file,fast_io::open::app|fast_io::open::binary>,basic_ostring<std::u8string>>;
using u8fsync_file = basic_fsync<basic_file_wrapper<native_file,fast_io::open::app|fast_io::open::binary>,basic_ostring<std::u8string>>;

using u8sync_file_mutex = basic_iomutex<u8sync_file>;
using u8fsync_file_mutex = basic_iomutex<u8fsync_file>;

using u8ibuf_file = basic_ibuf<u8inative_file>;
using u8obuf_file = basic_obuf<u8onative_file>;
using u8iobuf_file = basic_iobuf<u8ionative_file>;

using u8ibuf_file_mutex = basic_iomutex<u8ibuf_file>;
using u8obuf_file_mutex = basic_iomutex<u8obuf_file>;
using u8iobuf_file_mutex = basic_iomutex<u8iobuf_file>;

using u8dynamic_buf = basic_iobuf<u8dynamic_stream>;

}