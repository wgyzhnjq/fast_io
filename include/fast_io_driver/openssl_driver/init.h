#pragma once

namespace fast_io::openssl::details
{

class global_initializer
{
public:
	explicit global_initializer() noexcept
	{
		SSL_library_init();
	}
};
global_initializer ssl_init{};


}