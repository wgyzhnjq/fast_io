#pragma once

#include"../include/fast_io_device.h"
#include".,/include/fast_io_c_interface.h"

namespace fast_io
{
namespace
{
inline int lippincott_function()
{
	try
	{
		throw;
	}
	catch(std::system_error const& error)
	{
		if(error.code().category()==std::generic_category())
			return error.code().value();
		return EIO;
	}
	catch(...)
	{
		return EIO;
	}
}
}

extern "C"
{
int cxx_fast_io_read(char** readed,void* d,char* begin,char* end) noexcept
{
	try
	{
		fast_io::dynamic_base dyn(d);
		*readed=read(dyn,begin,end);
		return {};
	}
	catch(...)
	{
		return lippincott_function();
	}
}
int cxx_fast_io_write(char** written,void* d,char* begin,char* end) noexcept
{
	try
	{
		fast_io::dynamic_base dyn(d);
		*written=write(dyn,begin,end);
		return {};
	}
	catch(...)
	{
		return lippincott_function();
	}
}

}


}