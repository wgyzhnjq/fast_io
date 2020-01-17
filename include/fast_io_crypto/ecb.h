#pragma once

namespace fast_io::crypto
{

template<typename ciphert,bool Enc>
class ecb
{
public:
	using cipher_type = ciphert;
	using key_type = std::span<std::byte const, cipher_type::key_size>;
	using block_type = std::span<std::byte, cipher_type::block_size>;
	using iv_type = std::array<std::byte, cipher_type::block_size>;
	inline static constexpr std::size_t block_size = cipher_type::block_size;

public:
	cipher_type cipher;
	[[deprecated("ECB should NEVER EVER be used. The disadvantage of this method is a lack of diffusion. Because ECB encrypts identical plaintext blocks into identical ciphertext blocks, it does not hide data patterns well. In some senses, it doesn't provide serious message confidentiality, and it is not recommended for use in cryptographic protocols at all. https://en.wikipedia.org/wiki/Block_cipher_mode_of_operation#Electronic_Codebook_(ECB)")]] ecb(key_type key):cipher(key)
	{
	}
	inline auto operator()(std::span<std::byte, block_size> text)
	{
		return cipher(text.data());
	}
	auto digest(std::span<std::byte const> inp)
	{
		std::array<std::byte, block_size> plain_text{};
		details::my_copy(inp.begin(), inp.end(), plain_text.data());
		return cipher(plain_text.data());
	}
};

template<buffer_output_stream T, typename Enc, std::size_t sz = 4096>
using oecb_encrypt = otransform<T, block_processor<ecb<Enc,true>>, typename T::char_type, sz>;

template<buffer_input_stream T, typename Dec, std::size_t sz = 4096>
using iecb_decrypt = itransform<T, block_processor<ecb<Dec,false>>, typename T::char_type, sz>;

template<buffer_output_stream T, typename Dec, std::size_t sz = 4096>
using oecb_decrypt = otransform<T, block_processor<ecb<Dec,false>>, typename T::char_type, sz>;

template<buffer_input_stream T, typename Enc, std::size_t sz = 4096>
using iecb_encrypt = itransform<T, block_processor<ecb<Enc,true>>, typename T::char_type, sz>;

}
