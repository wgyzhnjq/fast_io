#pragma once

namespace fast_io::crypto
{

template<typename mcph>
class block_cipher
{
public:
	using mode_cipher_type = mcph;
	inline static constexpr std::size_t block_size = mode_cipher_type::block_size;
	mode_cipher_type cipher;
	template<typename... Args>
	requires std::constructible_from<mode_cipher_type,Args...>
	block_cipher(Args&& ...args):cipher(std::forward<Args>(args)...){}
	inline auto operator()(std::span<std::byte, block_size> text)
	{
		return cipher(text);
	}
	auto digest(std::span<std::byte const> inp)
	{
		std::array<std::byte, block_size> text{};
		details::my_copy(inp.begin(), inp.end(), text.data());
		return cipher(text);
	}
};

}