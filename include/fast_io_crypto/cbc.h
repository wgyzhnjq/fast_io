#pragma once

namespace fast_io::crypto
{

template<typename Enc>
class cbc_encrypt
{
public:
	using cipher_type = Enc;
	using unsigned_char_type = std::byte;
	using key_type = std::span<unsigned_char_type const, cipher_type::key_size>;
	using block_type = std::span<unsigned_char_type, cipher_type::block_size>;
	using iv_type = std::array<unsigned_char_type, cipher_type::block_size>;

public:
	iv_type iv;
	cipher_type enc;
	
	cbc_encrypt(key_type key, block_type iv2):enc(key)
	{
		details::my_copy_n(iv2.begin(), cipher_type::block_size, iv.data());
	}
	template<buffer_output_stream output, std::contiguous_iterator Iter>
	inline constexpr Iter write_proxy(output& out, Iter begin, Iter end)
	{
		while (end - begin >= cipher_type::block_size)
		{
			block_type plain_text(reinterpret_cast<std::byte*>(std::to_address(begin)), cipher_type::block_size);
			//details::my_copy_n(begin, cipher_type::block_size, plain_text.data());
			begin += cipher_type::block_size;
			for (std::size_t i{}; i != iv.size(); ++i)
				plain_text[i] ^= iv[i];
			auto cipher(enc(plain_text.data()));
			write(out, cipher.begin(), cipher.end());
			details::my_copy(cipher.begin(), cipher.end(), iv.data());
		}
		return begin;
	}
	template<buffer_output_stream output, std::contiguous_iterator Iter>
	inline constexpr void flush_proxy(output& out, Iter begin, Iter end)
	{
		begin = write_proxy(out, begin, end);
		auto plain_text(std::as_writable_bytes(std::span<char>(begin,end)));
		//details::my_copy_n(begin, diff, plain_text.data());
		for (std::size_t i{}; i != iv.size(); ++i)
			plain_text[i] ^= iv[i];
		auto cipher(enc(plain_text.data()));
		write(out, cipher.begin(), cipher.end());
		details::my_copy(cipher.begin(), cipher.end(), iv.data());
	}
	template<buffer_input_stream input, std::contiguous_iterator Iter>
	inline constexpr Iter read_proxy(input& in, Iter b, Iter e)
	{
		while (e - b >= cipher_type::block_size)
		{
			if (!ireserve(in, cipher_type::block_size))
				return b;
			std::span<unsigned_char_type, cipher_type::block_size> plain_text(begin(in));
			for (std::size_t i{}; i != iv.size(); ++i)
				plain_text[i] ^= iv[i];
			auto cipher(enc(plain_text.data()));
			details::my_copy_n(cipher.begin(), cipher_type::block_size, b);
			b += cipher_type::block_size;
			details::my_copy(cipher.begin(), cipher.end(), iv.data());
		}
		return b;
	}
};

template<typename Dec>
class cbc_decrypt
{
public:
	using cipher_type = Dec;
	using unsigned_char_type = std::byte;
	using key_type = std::span<unsigned_char_type const, cipher_type::key_size>;
	using block_type = std::span<unsigned_char_type, cipher_type::block_size>;
	using iv_type = std::array<unsigned_char_type, cipher_type::block_size>;

public:
	iv_type iv;
	cipher_type dec;
	cbc_decrypt(key_type key, block_type iv2):dec(key)
	{
		details::my_copy_n(iv2.begin(), cipher_type::block_size, iv.data());
	}
	template<buffer_output_stream output, std::contiguous_iterator Iter>
	inline constexpr Iter write_proxy(output& out, Iter begin, Iter end)
	{
		while (end - begin >= cipher_type::block_size)
		{
			block_type cipher_text(reinterpret_cast<std::byte*>(std::to_address(begin)), cipher_type::block_size);
			//details::my_copy_n(begin, cipher_type::block_size, cipher_text.data());
			begin += cipher_type::block_size;
			auto plain(dec(cipher_text.data()));
			for (std::size_t i{}; i != iv.size(); ++i)
				plain[i] ^= iv[i];
			write(out, plain.begin(), plain.end());
			details::my_copy_n(cipher_text.begin(), cipher_text.end(), iv.data());
		}
		return begin;
	}
	template<buffer_output_stream output, std::contiguous_iterator Iter>
	inline constexpr void flush_proxy(output& out, Iter begin, Iter end)
	{
		/*auto diff(end - begin);
		block_type plain_text{};
		my_copy_n(begin, diff, plain_text.data());
		for (std::size_t i{}; i != iv.size(); ++i)
			plain_text[i] ^= iv[i];
		auto cipher(enc(plain_text.data()));
		write(out, cipher.begin(), cipher.end());*/
	}
	template<buffer_input_stream input, std::contiguous_iterator Iter>
	inline constexpr Iter read_proxy(input& in, Iter b, Iter e)
	{
		auto be(std::as_writable_bytes(std::span<std::remove_reference_t<decltype(*b)>>(b, e)));
		while (be.size() >= cipher_type::block_size)
		{
			if (!ireserve(in, cipher_type::block_size))
				break;
			std::span<std::byte, cipher_type::block_size> cipher_text(reinterpret_cast<std::byte*>(std::to_address(begin(in))), cipher_type::block_size);
			auto plain(dec(cipher_text.data()));
			for (std::size_t i{}; i != iv.size(); ++i)
				plain[i] ^= iv[i];
			details::my_copy_n(plain.begin(), cipher_type::block_size, be.data());
			be = be.subspan(cipher_type::block_size);
			details::my_copy(cipher_text.begin(), cipher_text.end(), iv.data());
			in += cipher_type::block_size;
		}
		return e - be.size() / sizeof(*b);
	}
};

template<buffer_output_stream T,typename Enc,std::size_t sz=4096>
using ocbc_encrypt=otransform<T,cbc_encrypt<Enc>,typename T::char_type,sz>;
template<buffer_input_stream T,typename Dec,std::size_t sz=4096>
using icbc_decrypt=itransform<T,cbc_decrypt<Dec>,typename T::char_type,sz>;

} // namespace fast_io::crypto
