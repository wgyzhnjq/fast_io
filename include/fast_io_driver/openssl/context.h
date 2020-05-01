#pragma once

namespace fast_io::driver::openssl
{

using ssl_context_observer = details::observer<SSL_CTX*>

enum class method
{
tls,
tls_server,
tls_client,
datagram_tls,
datagram_server,
datagram_client,
};

namespace details
{
inline SSL_METHOD const* get_method(method m)
{
	switch(m)
	{
	case method::tls:
		return TLS_method();
	case method::tls_server:
		return TLS_server_method();
	case method::TLS_client_method:
		return TLS_client_method();
	case method::datagram_tls:
		return DTLS_method();
	case method::datagram_server:
		return DTLS_server_method();
	case method::datagram_client:
		return DTLS_client_method();
	default:
		throw std::runtime_error("unknown method");
	};
}
}

class ssl_context:public ssl_context_observer
{
public:
	ssl_context(openssl_context_observer ocb,
		std::string_view view,method m):ssl_context_observer(SSL_CTX_new(ocb.view.data(),details::get_method(m)))
	{
		if(this->native_handle()==nullptr)
			throw openssl_error("SSL_CTX_new() failed");
	}
	ssl_context(ssl_context const&) = delete;
	ssl_context& operator=(ssl_context const&) = delete;
	constexpr ssl_context(ssl_context&& bmv) noexcept:ssl_context_observer(bmv.native_handle())
	{
		bmv.native_handle()=nullptr;
	}
	ssl_context& operator=(ssl_context&& bmv) noexcept
	{
		if(bmv.native_handle()==this->native_handle())
			return *this;
		if(this->native_handle())[[likely]]
			SSL_CTX_free(this->native_handle());
		this->native_handle()=bmv->native_handle();
		bmv->native_handle()=nullptr;
		return *this;
	}
	~ssl_context()
	{
		if(this->native_handle())[[likely]]
			SSL_CTX_free(this->native_handle());
	}
};

}