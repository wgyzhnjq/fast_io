#pragma once

namespace fast_io::crypto
{

template<std::size_t N>
requires(N%128==0)
inline void fast_xor(std::span<std::byte,N> sp)
{

}

}