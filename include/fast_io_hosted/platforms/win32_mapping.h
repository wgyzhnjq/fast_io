#pragma once

namespace fast_io
{


enum class win32_file_map_attribute
{
copy=0x00000001,write=0x00000002,read=0x00000004,all_access=0x000f001f,execute=0x00000020
};

constexpr win32_file_map_attribute operator&(win32_file_map_attribute x, win32_file_map_attribute y) noexcept
{
using utype = typename std::underlying_type<win32_file_map_attribute>::type;
return static_cast<win32_file_map_attribute>(static_cast<utype>(x) & static_cast<utype>(y));
}

constexpr win32_file_map_attribute operator|(win32_file_map_attribute x, win32_file_map_attribute y) noexcept
{
using utype = typename std::underlying_type<win32_file_map_attribute>::type;
return static_cast<win32_file_map_attribute>(static_cast<utype>(x) | static_cast<utype>(y));
}

constexpr win32_file_map_attribute operator^(win32_file_map_attribute x, win32_file_map_attribute y) noexcept
{
using utype = typename std::underlying_type<win32_file_map_attribute>::type;
return static_cast<win32_file_map_attribute>(static_cast<utype>(x) ^ static_cast<utype>(y));
}

constexpr win32_file_map_attribute operator~(win32_file_map_attribute x) noexcept
{
using utype = typename std::underlying_type<win32_file_map_attribute>::type;
return static_cast<win32_file_map_attribute>(~static_cast<utype>(x));
}

inline win32_file_map_attribute& operator&=(win32_file_map_attribute& x, win32_file_map_attribute y) noexcept{return x=x&y;}

inline win32_file_map_attribute& operator|=(win32_file_map_attribute& x, win32_file_map_attribute y) noexcept{return x=x|y;}

inline win32_file_map_attribute& operator^=(win32_file_map_attribute& x, win32_file_map_attribute y) noexcept{return x=x^y;}

inline constexpr win32_file_map_attribute to_win32_file_map_attribute(file_map_attribute x)
{
	switch(x)
	{
	case file_map_attribute::execute_read:return win32_file_map_attribute::execute|win32_file_map_attribute::read;
	case file_map_attribute::execute_read_write:return win32_file_map_attribute::execute|win32_file_map_attribute::read;
	case file_map_attribute::execute_write_copy:return win32_file_map_attribute::execute|win32_file_map_attribute::write|win32_file_map_attribute::copy;
	case file_map_attribute::read_only:return win32_file_map_attribute::read;
	case file_map_attribute::read_write:return win32_file_map_attribute::read|win32_file_map_attribute::write;
	case file_map_attribute::write_copy:return win32_file_map_attribute::write|win32_file_map_attribute::copy;
	default:
		throw std::runtime_error("unknown file_mapping_attribute");
	};
}

class win32_file_mapping
{
	void* handle;
	void close_impl() noexcept
	{
		if(handle)
			win32::CloseHandle(handle);
	}
public:
	using native_handle_type = void*;
	template<std::integral ch_type>
	win32_file_mapping(basic_win32_file<ch_type>& bf,file_map_attribute attr,std::size_t size):
	handle(win32::CreateFileMappingA(
	bf.native_handle(),nullptr,static_cast<std::uint32_t>(attr),size>>32,static_cast<std::uint32_t>(size),nullptr))
	{
		if(handle==nullptr)
			throw win32_error();
	}
	win32_file_mapping(win32_file_mapping const&)=delete;
	win32_file_mapping& operator=(win32_file_mapping const&)=delete;
	win32_file_mapping(win32_file_mapping&& bmv) noexcept:handle(bmv.handle)
	{
		bmv.handle=nullptr;
	}
	win32_file_mapping& operator=(win32_file_mapping&& bmv) noexcept
	{
		if(handle==bmv.handle)
		{
			close_impl();
			handle=bmv.handle;
			bmv.handle=nullptr;
		}
		return *this;
	}
	void close() noexcept
	{
		close_impl();
		handle=nullptr;
	}
	auto native_handle() const
	{
		return handle;
	}
	~win32_file_mapping()
	{
		close_impl();
	}
};

class win32_map_view_of_file
{
	std::span<std::byte> rg;
public:
	win32_map_view_of_file(win32_file_mapping& wm,win32_file_map_attribute attr,std::size_t bytes,std::size_t start_address=0):
			rg({reinterpret_cast<std::byte*>(win32::MapViewOfFile(wm.native_handle(),static_cast<std::uint32_t>(attr),
			start_address>>32,static_cast<std::uint32_t>(start_address),bytes)),bytes})
	{
		if(rg.data()==nullptr)
			throw win32_error();
	}
	win32_map_view_of_file(win32_map_view_of_file const&)=delete;
	win32_map_view_of_file& operator=(win32_map_view_of_file const&)=delete;
	win32_map_view_of_file(win32_map_view_of_file&& wm) noexcept:rg(wm.rg)
	{
		wm.rg={};
	}
	win32_map_view_of_file& operator=(win32_map_view_of_file&& wm) noexcept
	{
		if(std::addressof(wm)==this)
		{
			if(rg.data())
				win32::UnmapViewOfFile(rg.data());
			rg=wm.rg;
			wm.rg={};
		}
		return *this;
	}
	auto& region()
	{
		return rg;
	}
	void close() noexcept
	{
		if(rg.data())
			win32::UnmapViewOfFile(rg.data());
		rg={};
	}
	~win32_map_view_of_file()
	{
		if(rg.data())
			win32::UnmapViewOfFile(rg.data());
	}
};


class win32_file_map
{
	win32_file_mapping wfm;
	win32_map_view_of_file view;
public:
	template<std::integral ch_type>
	win32_file_map(basic_win32_file<ch_type>& bf,file_map_attribute attr,std::size_t bytes,std::size_t start_address=0):
		wfm(bf,attr,bytes),view(wfm,to_win32_file_map_attribute(attr),bytes,start_address){}
	auto native_handle() const {return wfm.native_handle();}
	auto& region()
	{
		return view.region();
	}
	void close() noexcept
	{
		wfm.close();
		view.close();
	}
};


class omap
{
public:
	using native_handle_type = win32_file;
private:
	native_handle_type hd;
	win32_file_map fm;
public:
	std::size_t current_position{};
public:
	using char_type = char;//only char is allowed for punning
	template<typename ...Args>
	requires std::constructible_from<native_handle_type,Args...>
	omap(Args&& ...args):hd(std::forward<Args>(args)...),fm(hd,fast_io::file_map_attribute::read_write,16384)
	{}
	auto& native_handle()
	{
		return hd;
	}
	auto& map_handle()
	{
		return fm;
	}
	omap(omap const&)=delete;
	omap& operator=(omap const&)=delete;
	omap(omap &&om) noexcept:hd(std::move(om.hd)),fm(std::move(om.fm)),current_position(om.current_position){}
	omap& operator=(omap&& om) noexcept
	{
		if(std::addressof(om)!=this)
		{
			hd=std::move(om.hd);
			fm=std::move(om.fm);
			current_position=om.current_position;
		}
		return *this;
	}
	auto& region()
	{
		return fm.region();
	}
	~omap()
	{
		fm.close();
//		if(valid(hd))
//			truncate(hd,current_position);
	}
};

inline std::size_t size(omap& om)
{
	return om.region().size();
}

inline void reserve(omap& om,std::size_t trunc)
{
	om.map_handle()=win32_file_map(om.native_handle(),fast_io::file_map_attribute::read_write,trunc);
//	om.map_handle().reopen(om.native_handle(),fast_io::file_map_attribute::read_write,trunc);
	if(trunc<om.current_position)
		om.current_position=trunc;
}

namespace details
{
inline void grow_omap(omap& om,std::size_t sz,std::size_t n)
{
	std::size_t const db(sz<<1);
	if(db<n)
		reserve(om,n);
	else
		reserve(om,db);
}
}

template<std::contiguous_iterator Iter>
inline void send(omap& om,Iter cbegin,Iter cend)
{
//http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p1072r2.html
//strict aliasing rule
	std::size_t const bytes(sizeof(*cbegin)*(cend-cbegin));
	auto& region(om.region());
	std::size_t after(om.current_position+bytes);
	if(region.size()<=after)[[unlikely]]
		details::grow_omap(om,region.size(),after);
	memcpy(region.data()+om.current_position,std::to_address(cbegin),bytes);
	om.current_position=after;
}

inline void flush(omap&){}

}