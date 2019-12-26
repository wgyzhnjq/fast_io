#pragma once

#include <sys/mman.h>

namespace fast_io
{


enum class posix_file_map_attribute
{
none=PROT_NONE,write=PROT_WRITE,execute=PROT_EXEC,read=PROT_READ
};

constexpr posix_file_map_attribute operator&(posix_file_map_attribute x, posix_file_map_attribute y) noexcept
{
using utype = typename std::underlying_type<posix_file_map_attribute>::type;
return static_cast<posix_file_map_attribute>(static_cast<utype>(x) & static_cast<utype>(y));
}

constexpr posix_file_map_attribute operator|(posix_file_map_attribute x, posix_file_map_attribute y) noexcept
{
using utype = typename std::underlying_type<posix_file_map_attribute>::type;
return static_cast<posix_file_map_attribute>(static_cast<utype>(x) | static_cast<utype>(y));
}

constexpr posix_file_map_attribute operator^(posix_file_map_attribute x, posix_file_map_attribute y) noexcept
{
using utype = typename std::underlying_type<posix_file_map_attribute>::type;
return static_cast<posix_file_map_attribute>(static_cast<utype>(x) ^ static_cast<utype>(y));
}

constexpr posix_file_map_attribute operator~(posix_file_map_attribute x) noexcept
{
using utype = typename std::underlying_type<posix_file_map_attribute>::type;
return static_cast<posix_file_map_attribute>(~static_cast<utype>(x));
}

inline posix_file_map_attribute& operator&=(posix_file_map_attribute& x, posix_file_map_attribute y) noexcept{return x=x&y;}

inline posix_file_map_attribute& operator|=(posix_file_map_attribute& x, posix_file_map_attribute y) noexcept{return x=x|y;}

inline posix_file_map_attribute& operator^=(posix_file_map_attribute& x, posix_file_map_attribute y) noexcept{return x=x^y;}

inline constexpr posix_file_map_attribute to_posix_file_map_attribute(file_map_attribute x)
{
	switch(x)
	{
	case file_map_attribute::execute_read:return posix_file_map_attribute::execute|posix_file_map_attribute::read;
	case file_map_attribute::execute_read_write:return posix_file_map_attribute::execute|posix_file_map_attribute::read;
	case file_map_attribute::execute_write_copy:return posix_file_map_attribute::execute|posix_file_map_attribute::write;
	case file_map_attribute::read_only:return posix_file_map_attribute::read;
	case file_map_attribute::read_write:return posix_file_map_attribute::read|posix_file_map_attribute::write;
	case file_map_attribute::write_copy:return posix_file_map_attribute::write;
	default:
		throw std::runtime_error("unknown file_mapping_attribute");
	};
}



class posix_file_map
{
    std::span<std::byte> rg;
public:
	template<std::integral ch_type>
	posix_file_map(basic_posix_file<ch_type>& bf,file_map_attribute attr,std::size_t bytes,std::size_t start_address=0):
		rg(static_cast<std::byte*>(mmap(nullptr, bytes, static_cast<int>(to_posix_file_map_attribute(attr)), MAP_SHARED, bf.native_handle(), start_address)), bytes)
	{
		if (rg.data() == MAP_FAILED)
			throw std::system_error(errno,std::generic_category());
	}
	//auto native_handle() const {return wfm.native_handle();}
	auto region()
	{
		return rg;
	}
	posix_file_map(posix_file_map const&)=delete;
	posix_file_map& operator=(posix_file_map const&)=delete;
	posix_file_map(posix_file_map&& pm) noexcept:rg(pm.rg)
	{
		rg={};
	}
	posix_file_map& operator=(posix_file_map&& pm) noexcept
	{
		if(std::addressof(pm)==this)
		{
			if(rg.data())
				munmap(rg.data(), rg.size());
			rg=pm.rg;
			pm.rg={};
		}
		return *this;
	}
	~posix_file_map()
	{
		if(rg.data())
			munmap(rg.data(), rg.size());
	}
};



}
