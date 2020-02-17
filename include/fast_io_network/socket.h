#pragma once

namespace fast_io
{

template<bool async=false>
class basic_socket
{
	sock::details::socket_type handle=sock::details::invalid_socket;
	void close_impl()
	{
#ifdef __cpp_exceptions
	try
	{
#endif
		if(handle!=sock::details::invalid_socket)
			sock::details::closesocket(handle);
#ifdef __cpp_exceptions
	}
	catch(...)
	{}
#endif
	}
public:
	basic_socket()=default;
	basic_socket(sock::details::socket_type v):handle(v){}
	template<typename ...Args>
	basic_socket(native_interface_t,Args&& ...args):handle(sock::details::socket(std::forward<Args>(args)...)){}
	basic_socket(sock::family family,sock::type const &type,sock::protocal const &protocal = sock::protocal::none):
		handle(sock::details::socket(static_cast<sock::details::address_family>(family),static_cast<int>(type),static_cast<int>(protocal))){}
	auto& native_handle() {return handle;}
	basic_socket(basic_socket const&) = delete;
	basic_socket& operator=(basic_socket const&) = delete;
	basic_socket(basic_socket && soc) noexcept:handle(soc.handle)
	{
		soc.handle = sock::details::invalid_socket;
	}
	basic_socket& operator=(basic_socket && soc) noexcept
	{
		if(soc.handle!=handle)
		{
			close_impl();
			handle = soc.handle;
			soc.handle = sock::details::invalid_socket;
		}
		return *this;
	}
	~basic_socket()
	{
		close_impl();
	}
};
template<bool async>
inline constexpr void flush(basic_socket<async>&)
{
}
#ifdef __linux__
inline auto zero_copy_in_handle(basic_socket<false>& soc)
{
	return soc.native_handle();
}

inline auto zero_copy_out_handle(basic_socket<false>& soc)
{
	return soc.native_handle();
}
#endif

#if defined(__WINNT__) || defined(_MSC_VER)
/*
Bug to be fixed. disable first
inline auto zero_copy_out_handle(basic_socket<false>& soc)
{
	return soc.native_handle();
}

*/
#endif

struct address_info
{
	socket_address_storage storage={};
	socklen_t storage_size=sizeof(socket_address_storage);
};

template<bool async=false>
class basic_connected_socket:public basic_socket<async>
{
public:
	template<typename ...Args>
	requires std::constructible_from<basic_socket<async>,Args...>
	basic_connected_socket(Args&& ...args):basic_socket<async>(std::forward<Args>(args)...){}
};

template<bool async,std::contiguous_iterator Iter>
inline Iter read(basic_connected_socket<async>& soc,Iter begin,Iter end)
{
	return begin+((sock::details::recv(soc.native_handle(),std::to_address(begin),static_cast<int>((end-begin)*sizeof(*begin)),0))/sizeof(*begin));
}
template<bool async,std::contiguous_iterator Iter>
inline Iter write(basic_connected_socket<async>& soc,Iter begin,Iter end)
{
	return begin+(sock::details::send(soc.native_handle(),std::to_address(begin),static_cast<int>((end-begin)*sizeof(*begin)),0)/sizeof(*begin));
}

template<bool async>
inline auto redirect_handle(basic_connected_socket<async>& soc)
{
	return soc.native_handle();
}

template<bool async=false>
class basic_connected_server
{
	basic_connected_socket<async> soc;
public:
	template<typename addrType,std::integral U,typename ...Args>
	requires (!std::integral<addrType>)
	basic_connected_server(addrType const& add,U u,Args&& ...args):soc(family(add),std::forward<Args>(args)...)
	{
		auto stg(to_socket_address_storage(add,u));
		sock::details::bind(soc.native_handle(),stg,native_socket_address_size(add));
		sock::details::listen(soc.native_handle(),10);
	}
	constexpr auto& native_handle()
	{
		return soc;
	}
};
template<bool async=false>
class basic_tcp_server:public basic_connected_server<async>
{
public:
	template<typename addrType,std::integral U>
	requires (!std::integral<addrType>)
	basic_tcp_server(addrType const& add,U u):basic_connected_server<async>(family(add),u,fast_io::sock::type::stream){}
	template<std::integral U>
	basic_tcp_server(U u):basic_connected_server<async>(fast_io::ipv4{},u,fast_io::sock::type::stream){}
};


#if defined(__WINNT__) || defined(_MSC_VER)
#else
inline void unblock(basic_socket<true>& sv)
{
	if(::fcntl(sv.native_handle(), F_SETFL, O_NONBLOCK)==-1)
#ifdef __cpp_exceptions
		throw std::system_error(errno,std::generic_category());
#else
		fast_terminate();
#endif
}

inline void unblock(basic_connected_server<true>& sv)
{
	unblock(sv.native_handle());
}
#endif

template<std::integral ch_type,bool async=false>
class basic_acceptor:public basic_connected_socket<async>
{
	address_info cinfo;
public:
	using native_handle_type = sock::details::socket_type;
	using char_type = ch_type;
	using basic_connected_socket<async>::native_handle;
	basic_acceptor(basic_connected_server<async>& listener_socket)
	{
		native_handle()=sock::details::accept(listener_socket.native_handle().native_handle(),cinfo.storage,cinfo.storage_size);
		if constexpr(async)
			unblock(*this);
	}
/*
	basic_acceptor(async_server& listener_socket)
	{
#if defined(__WINNT__) || defined(_MSC_VER)
		native_handle()=sock::details::accept(listener_socket.native_handle().native_handle(),cinfo.storage,cinfo.storage_size);
#else
		native_handle()=sock::details::accept(listener_socket.native_handle().native_handle(),cinfo.storage,cinfo.storage_size);
		unblock(*this);
#endif
	}*/
	constexpr auto& info()
	{
		return cinfo;
	}
	constexpr auto& info() const
	{
		return cinfo;
	}
};


template<std::integral ch_type,bool async=false>
class basic_connected_client:public basic_connected_socket<async>
{
	address_info cinfo;
public:
	using char_type = ch_type;
	using basic_connected_socket<async>::native_handle;
	template<typename T,std::integral U,typename ...Args>
	basic_connected_client(T const& add,U u,Args&& ...args):basic_connected_socket<async>(family(add),std::forward<Args>(args)...),cinfo{to_socket_address_storage(add,u),sizeof(socket_address_storage)}
	{
		sock::details::connect(native_handle(),cinfo.storage,native_socket_address_size(add));
	}
	constexpr auto& info()
	{
		return cinfo;
	}
	constexpr auto& info() const
	{
		return cinfo;
	}
};

template<std::integral ch_type,bool async=false>
class basic_tcp_client:public basic_connected_client<ch_type,async>
{
public:
	using char_type = ch_type;
	template<typename T,std::integral U>
	basic_tcp_client(T const& add,U u):basic_connected_client<ch_type,async>(add,u,fast_io::sock::type::stream){}
};
/*
class no_connection_socket:public socket
{
public:
	using char_type = ch_type;
	template<typename T,std::integral U,typename ...Args>
	no_connection_socket(fast_io::sock::family fam):socket(fam,){}
};
*/

}