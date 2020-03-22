#pragma once

namespace fast_io
{



#if defined(__GNU_LIBRARY__)
inline char* oreserve(c_io_observer_unlocked& cfhd,std::size_t n)
{
	auto& h(*cfhd.native_handle());
	if(h._IO_write_end<=h._IO_write_ptr+n)
		return nullptr;
	return h._IO_write_ptr+=n;
}

inline void orelease(c_io_observer_unlocked& cfhd,std::size_t n)
{
	cfhd.native_handle()->_IO_write_ptr-=n;
}
#endif
}