#pragma once

//fast_io_network.h deals with sockets
#include"fast_io_hosted.h"
#include"fast_io_network/network.h"

namespace fast_io
{
using acceptor = basic_acceptor<char>;
using tcp_client = basic_tcp_client<char>;
using ibuf_acceptor = basic_ibuf<acceptor>;
using ibuf_tcp_client = basic_ibuf<tcp_client>;
/*
using u8tcp_acceptor = basic_acceptor<char8_t>;
using u8tcp_client = basic_client<char8_t>;
using u8tcp_acceptor_buf = basic_ibuf<u8acceptor>;
using u8tcp_client_buf = basic_ibuf<u8client>;*/
}