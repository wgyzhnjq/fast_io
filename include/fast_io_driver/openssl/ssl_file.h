#pragma once

namespace fast_io::driver::openssl
{

template<std::integral ch_type>
class basic_ssl_io_observer
{
public:
	using char_type = ch_type;
	using native_handle_type = SSL*;
	native_handle_type s{};
	constexpr auto& native_handle() const noexcept
	{
		return s;
	}
	constexpr auto& native_handle() noexcept
	{
		return s;
	}
	constexpr operator bool() noexcept
	{
		return s;
	}
};

template<std::integral ch_type>
inline std::size_t use_count(basic_ssl_io_observer<ch_type> siob)
{
	return static_cast<std::size_t>(SSL_up_ref(siob.s));
}

template<std::integral ch_type>
class basic_ssl_io_handle:public basic_ssl_io_observer<ch_type>
{
public:
	using char_type = ch_type;
	using native_handle_type = SSL*;
	constexpr basic_ssl_io_handle()=default;
	constexpr basic_ssl_io_handle(native_handle_type s):basic_ssl_io_observer<ch_type>(s){}
	basic_ssl_io_handle(basic_ssl_io_handle const& h):basic_ssl_io_observer<ch_type>(SSL_dup(h.native_handle()))
	{
		if(this->native_handle()==nullptr)[[unlikely]]
			throw openssl_error("SSL_dup() failed");
	}
	basic_ssl_io_handle& operator=(basic_ssl_io_handle const& h)
	{
		auto temp{SSL_dup(h.native_handle())}:
		if(temp==nullptr)[[unlikely]]
			throw openssl_error("SSL_dup() failed");
		if(this->native_handle())[[likely]]
			SSL_free(this->native_handle());
		this->native_handle()=temp;
		return *this;
	}
	constexpr basic_ssl_io_handle(basic_ssl_io_handle&& h) noexcept:basic_ssl_io_observer<ch_type>(h.native_handle())
	{
		h.native_handle()=nullptr;
	}
	basic_ssl_io_handle& operator=(basic_ssl_io_handle&& h) noexcept
	{
		if(h.native_handle()==this->native_handle())
			return *this;
		if(this->native_handle())[[likely]]
			SSL_free(this->native_handle());
		this->native_handle()=h.native_handle();
		h.native_handle()=nullptr;
		return *this;
	}
};

template<std::integral ch_type>
class basic_ssl_file:public basic_ssl_io_handle<ch_type>
{
public:
	using char_type = ch_type;
	using native_handle_type = SSL*;
	constexpr basic_ssl_file()=default;
	constexpr basic_ssl_file(native_handle_type s):basic_ssl_io_handle<ch_type>(s){}

	basic_ssl_file(ssl_context_observer ssl_ctx_ob):basic_ssl_io_handle<ch_type>(SSL_new(ssl_ctx_ob.native_handle()))
	{
		if(this->native_handle()==nullptr)
			throw openssl_error("SSL_new() failed");
	}

	~basic_ssl_file()
	{
		if(this->native_handle())[[likely]]
			SSL_free(this->native_handle());
	}
};

}