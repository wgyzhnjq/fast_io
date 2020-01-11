#pragma once

//fast_io_network.h deals with sockets
#include"fast_io_hosted.h"
#include"fast_io_network/network.h"

namespace fast_io
{
using acceptor = basic_acceptor<char>;
using client = basic_client<char>;
using acceptor_buf = basic_ibuf<acceptor>;
using client_buf = basic_ibuf<client>;

using u8acceptor = basic_acceptor<char8_t>;
using u8client = basic_client<char8_t>;
using u8acceptor_buf = basic_ibuf<u8acceptor>;
using u8client_buf = basic_ibuf<u8client>;
}