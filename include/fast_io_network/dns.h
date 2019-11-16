#pragma once

#include <variant>

namespace fast_io
{

struct ipv4
{
	std::array<std::uint8_t, 4> address{};
};

template<character_output_stream output>
inline constexpr void print_define(output& os, ipv4 const &v)
{
	print(os, v.address.front());
	put(os, '.');
	print(os, v.address[1]);
	put(os, '.');
	print(os, v.address[2]);
	put(os, '.');
	print(os, v.address.back());
}

struct ipv6
{
	std::array<std::uint8_t, 16> address{};
};

template<character_output_stream output>
inline constexpr void print_define(output &os, ipv6 const &v)
{
	print(os, fast_io::hex(v.address.front()));
	for (auto i(v.address.cbegin() + 1); i != v.address.cend(); ++i)
	{
		put(os, ':');
		print(os, fast_io::hex(*i));
	}
}

class addres
{
public:
	using variant_type = std::variant<ipv4, ipv6>;
private:
	variant_type var;
public:
	template<typename... Args>
	requires std::constructible_from<variant_type, Args...>
	addres(Args &&... args) :var(std::forward<Args>(args)...){}

	auto& variant() { return var; }
	auto& variant() const { return var; }
};

template<character_output_stream output>
inline constexpr void print_define(output &os, addres const &v)
{
	std::visit([&os](auto const &arg) {
		print_define(os,arg);
	}, v.variant());
}

namespace details
{
	struct dns_iter
	{
		addrinfo *ptr;
	};
	struct dns_sentinal
	{
	};
	inline constexpr bool operator==(dns_iter const& a, dns_iter const& b)
	{
		return a.ptr == b.ptr;
	}
	inline constexpr bool operator!=(dns_iter const& a, dns_iter const& b)
	{
		return !(a==b);
	}
	inline constexpr bool operator==(dns_sentinal const& a, dns_iter const& b)
	{
		return b.ptr == nullptr;
	}
	inline constexpr bool operator==(dns_iter const& b, dns_sentinal const& a)
	{
		return b.ptr == nullptr;
	}
	inline constexpr bool operator!=(dns_sentinal const& a, dns_iter const& b)
	{
		return !(b==a);
	}
	inline constexpr bool operator!=(dns_iter const& b, dns_sentinal const& a)
	{
		return !(b==a);
	}
	inline addres operator*(dns_iter const &a)
	{
		if (a.ptr->ai_family == AF_INET)
		{
			sockaddr_in addr;
			memcpy(std::addressof(addr), a.ptr->ai_addr, sizeof(addr));
			ipv4 ret;
			memcpy(ret.address.data(), std::addressof(addr.sin_addr), sizeof(ret.address));
			return addres(ret);
		}
		else if (a.ptr->ai_family == AF_INET6)
		{
			sockaddr_in6 addr;
			memcpy(std::addressof(addr), a.ptr->ai_addr, sizeof(addr));
			ipv6 ret;
			memcpy(ret.address.data(), std::addressof(addr.sin6_addr), sizeof(ret.address));
			return addres(ret);
		}
		return {};
	}
	inline constexpr dns_iter& operator++(dns_iter& a)
	{
		a.ptr = a.ptr->ai_next;
		return a;
	}
	inline constexpr dns_iter operator++(dns_iter& a, int)
	{
		auto temp(a);
		++a;
		return temp;
	}
}

template<fast_io::sock::family fam>
class basic_dns
{
	addrinfo *result;
public:
	basic_dns(std::string_view host)
	{
		addrinfo hints{};
		hints.ai_family = static_cast<fast_io::sock::details::address_family>(fam);
		fast_io::sock::details::getaddrinfo(host.data(), nullptr, std::addressof(hints), std::addressof(result));
	}
	constexpr auto& get_result()
	{
		return result;
	}
	constexpr auto& get_result() const
	{
		return result;
	}
};

using ipv4_dns = basic_dns<fast_io::sock::family::ipv4>;
using ipv6_dns = basic_dns<fast_io::sock::family::ipv6>;
using dns = basic_dns<fast_io::sock::family::unspec>;

template<fast_io::sock::family fam>
inline constexpr auto cbegin(basic_dns<fam> const& d)
{
	return details::dns_iter{d.get_result()};
}

template<fast_io::sock::family fam>
inline constexpr auto begin(basic_dns<fam> const& d)
{
	return cbegin(d);
}

template<fast_io::sock::family fam>
inline constexpr auto begin(basic_dns<fam> & d)
{
	return details::dns_iter{d.get_result()};
}

template<fast_io::sock::family fam>
inline constexpr details::dns_sentinal end(basic_dns<fam> const& d)
{
	return {};
}

template<fast_io::sock::family fam>
inline constexpr details::dns_sentinal cend(basic_dns<fam> const& d)
{
	return {};
}

}