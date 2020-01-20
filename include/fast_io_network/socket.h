#pragma once

namespace fast_io
{

class socket
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
	socket()=default;
	socket(sock::details::socket_type v):handle(v){}
	template<typename ...Args>
	socket(native_interface_t,Args&& ...args):handle(sock::details::socket(std::forward<Args>(args)...)){}
	socket(sock::family family,sock::type const &type,sock::protocal const &protocal = sock::protocal::none):
		handle(sock::details::socket(static_cast<sock::details::address_family>(family),static_cast<int>(type),static_cast<int>(protocal))){}
	auto& native_handle() {return handle;}
	socket(socket const&) = delete;
	socket& operator=(socket const&) = delete;
	socket(socket && soc) noexcept:handle(soc.handle)
	{
		soc.handle = sock::details::invalid_socket;
	}
	socket& operator=(socket && soc) noexcept
	{
		if(soc.handle!=handle)
		{
			close_impl();
			handle = soc.handle;
			soc.handle = sock::details::invalid_socket;
		}
		return *this;
	}
	~socket()
	{
		close_impl();
	}
};

inline constexpr void flush(socket&)
{
}
#ifdef __linux__
inline auto zero_copy_in_handle(socket& soc)
{
	return soc.native_handle();
}

inline auto zero_copy_out_handle(socket& soc)
{
	return soc.native_handle();
}
#endif

#if defined(__WINNT__) || defined(_MSC_VER)
/*
Bug to be fixed. disable first
inline auto zero_copy_out_handle(socket& soc)
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

class connected_socket:public socket
{
public:
	template<typename ...Args>
	requires std::constructible_from<socket,Args...>
	connected_socket(Args&& ...args):socket(std::forward<Args>(args)...){}
};

template<std::contiguous_iterator Iter>
inline Iter read(connected_socket& soc,Iter begin,Iter end)
{
	return begin+((sock::details::recv(soc.native_handle(),std::to_address(begin),static_cast<int>((end-begin)*sizeof(*begin)),0))/sizeof(*begin));
}
template<std::contiguous_iterator Iter>
inline Iter write(connected_socket& soc,Iter begin,Iter end)
{
	return begin+(sock::details::send(soc.native_handle(),std::to_address(begin),static_cast<int>((end-begin)*sizeof(*begin)),0)/sizeof(*begin));
}

class connected_server
{
	connected_socket soc;
public:
	template<typename addrType,std::integral U,typename ...Args>
	requires (!std::integral<addrType>)
	connected_server(addrType const& add,U u,Args&& ...args):soc(family(add),std::forward<Args>(args)...)
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

class tcp_server:public connected_server
{
public:
	template<typename addrType,std::integral U>
	requires (!std::integral<addrType>)
	tcp_server(addrType const& add,U u):connected_server(family(add),u,fast_io::sock::type::stream){}
	template<std::integral U>
	tcp_server(U u):connected_server(fast_io::ipv4{},u,fast_io::sock::type::stream){}
};


#if defined(__WINNT__) || defined(_MSC_VER)
#else
inline void unblock(socket& sv)
{
	if(::fcntl(sv.native_handle(), F_SETFL, O_NONBLOCK)==-1)
#ifdef __cpp_exceptions
		throw std::system_error(errno,std::generic_category());
#else
		fast_terminate();
#endif
}

inline void unblock(server& sv)
{
	unblock(sv.native_handle());
}
#endif

template<std::integral ch_type>
class basic_acceptor:public connected_socket
{
	address_info cinfo;
public:
	using native_handle_type = sock::details::socket_type;
	using char_type = ch_type;
	basic_acceptor(connected_server& listener_socket)
	{
		native_handle()=sock::details::accept(listener_socket.native_handle().native_handle(),cinfo.storage,cinfo.storage_size);
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


template<std::integral ch_type>
class basic_connected_client:public connected_socket
{
	address_info cinfo;
public:
	using char_type = ch_type;
	template<typename T,std::integral U,typename ...Args>
	basic_connected_client(T const& add,U u,Args&& ...args):connected_socket(family(add),std::forward<Args>(args)...),cinfo{to_socket_address_storage(add,u),sizeof(socket_address_storage)}
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

template<std::integral ch_type>
class basic_tcp_client:public basic_connected_client<ch_type>
{
public:
	using char_type = ch_type;
	template<typename T,std::integral U>
	basic_tcp_client(T const& add,U u):basic_connected_client<ch_type>(add,u,fast_io::sock::type::stream){}
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