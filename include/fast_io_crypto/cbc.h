#pragma once

namespace fast_io::crypto
{

template<typename Enc>
class [[deprecated("CBC is no longer secure due to a padding oracle attack. https://en.wikipedia.org/wiki/Padding_oracle_attack#Padding_oracle_attack_on_CBC_encryption")]] cbc_encrypt
{
public:
	using cipher_type = Enc;
	using unsigned_char_type = std::byte;
	using key_type = std::span<unsigned_char_type const, cipher_type::key_size>;
	using block_type = std::span<unsigned_char_type, cipher_type::block_size>;
	using iv_type = std::array<unsigned_char_type, cipher_type::block_size>;
	inline static constexpr std::size_t block_size = cipher_type::block_size;

public:
	iv_type iv;
	cipher_type enc;
	
	cbc_encrypt(key_type key, block_type iv2):enc(key)
	{
		details::my_copy_n(iv2.begin(), cipher_type::block_size, iv.data());
	}

    auto operator()(std::span<std::byte, block_size> plain_text)
    {
        for (std::size_t i{}; i != iv.size(); ++i)
			plain_text[i] ^= iv[i];
        auto cipher(enc(plain_text.data()));
        details::my_copy(cipher.begin(), cipher.end(), iv.data());
        return cipher;
    }

    auto digest(std::span<std::byte const> inp)
    {
        std::array<std::byte, block_size> plain_text{};
        details::my_copy(inp.begin(), inp.end(), plain_text.data());
        for (std::size_t i{}; i != iv.size(); ++i)
			plain_text[i] ^= iv[i];
        auto cipher(enc(plain_text.data()));
        details::my_copy(cipher.begin(), cipher.end(), iv.data());
        return cipher;
    }
};

template<typename Dec>
class [[deprecated("CBC is no longer secure due to a padding oracle attack. https://en.wikipedia.org/wiki/Padding_oracle_attack#Padding_oracle_attack_on_CBC_encryption")]] cbc_decrypt
{
public:
	using cipher_type = Dec;
	using unsigned_char_type = std::byte;
	using key_type = std::span<unsigned_char_type const, cipher_type::key_size>;
	using block_type = std::span<unsigned_char_type, cipher_type::block_size>;
	using iv_type = std::array<unsigned_char_type, cipher_type::block_size>;
	inline static constexpr std::size_t block_size = cipher_type::block_size;

public:
	iv_type iv;
	cipher_type dec;
	
	cbc_decrypt(key_type key, block_type iv2):dec(key)
	{
		details::my_copy_n(iv2.begin(), cipher_type::block_size, iv.data());
	}

    auto operator()(std::span<std::byte, block_size> cipher_text)
    {
        auto plain(dec(cipher_text.data()));
        for (std::size_t i{}; i != iv.size(); ++i)
			plain[i] ^= iv[i];
        details::my_copy(cipher_text.begin(), cipher_text.end(), iv.data());
        return plain;
    }
};

template<buffer_output_stream T, typename Enc, std::size_t sz = 4096>
using ocbc_encrypt = otransform<T, block_processor<cbc_encrypt<Enc>>, typename T::char_type, sz>;

template<buffer_input_stream T, typename Dec, std::size_t sz = 4096>
using icbc_decrypt = itransform<T, block_processor<cbc_decrypt<Dec>>, typename T::char_type, sz>;

template<buffer_output_stream T, typename Dec, std::size_t sz = 4096>
using ocbc_decrypt = otransform<T, block_processor<cbc_decrypt<Dec>>, typename T::char_type, sz>;

template<buffer_input_stream T, typename Enc, std::size_t sz = 4096>
using icbc_encrypt = itransform<T, block_processor<cbc_encrypt<Enc>>, typename T::char_type, sz>;

}
