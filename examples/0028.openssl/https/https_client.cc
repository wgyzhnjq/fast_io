#include"../../../include/fast_io.h"
#include"../../../include/fast_io_driver/openssl_driver.h"

int main()
{
	fast_io::openssl::ssl_context ssl_ctx(fast_io::openssl::tls_method::tls);
	fast_io::openssl::ssl_file file(ssl_ctx);
}