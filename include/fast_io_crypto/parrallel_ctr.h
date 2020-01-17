#pragma once

#include<jthread>

namespace fast_io
{

template<typename ciphert,std::size_t thread_number=8,bool big_endian = false>
class parrallel_ctr
{
public:
	using cipher_type = ciphert;
	using key_type = std::span<std::byte const, cipher_type::key_size>;
	using block_type = std::span<std::byte, cipher_type::block_size>;
	using nonce_type = std::span<std::byte const, cipher_type::block_size-8>;
	using counter_type = std::uint64_t;
	inline static constexpr std::size_t block_size = cipher_type::block_size*thread_number;
	cipher_type cipher;
	std::array<std::jthread,thread_number> jthreads;
	alignas(16) std::array<std::byte,cipher_type::block_size-8> nonce_block{};
	std::size_t counter{};
public:
	parrallel_ctr(key_type key, nonce_type nc):cipher(key)
	{
		details::my_copy(nc.begin(), nc.end(), nonce_block.data());
	}
	parrallel_ctr(key_type key, nonce_type nc, std::uint64_t start_counter):cipher(key),counter(start_counter)
	{
		details::my_copy(nc.begin(), nc.end(), nonce_block.data());
	}
	inline void operator()(std::span<std::byte, block_size> text)
	{
		for(auto& th : threads)
		{

		}
	}
}

}