#pragma once

namespace fast_io::driver::openssl::details
{

class global_initializer
{
public:
	explicit openssl_global_initializer() noexcept
	{
		SSL_library_init();
	}
};
global_initializer ssl_init{};


}