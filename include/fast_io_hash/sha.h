#pragma once

namespace fast_io
{

template<typename T>
struct sha
{
public:
	using char_type = char;
	using function_type = T;
	typename T::digest_type digest = T::digest_initial_value;
	typename T::block_type block{};
	T function;
	std::size_t transform_counter{};
	inline static constexpr std::size_t block_size = block.size();
	void operator()(std::span<std::byte const,block_size> process_block)
	{
		if constexpr(std::endian::native==std::endian::little)
			function(process_block.data());
		else
			function(process_block.data());
		++transform_counter;
	}
	void digest(std::span<std::byte const,block_size> final_block)
	{

	}
};


}