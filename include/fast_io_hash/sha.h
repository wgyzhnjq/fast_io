#pragma once

namespace fast_io
{

template<typename T>
struct sha
{
public:
	using char_type = char;
	using function_type = T;
	typename T::digest_type digest_block = T::digest_initial_value;
	inline static constexpr std::size_t block_size = T::block_size;
	T function;
	std::size_t transform_counter{};
	void operator()(std::span<std::byte const,block_size> process_block)
	{
		function(digest_block,process_block);
		++transform_counter;
	}
	void digest(std::span<std::byte const> final_block)//contracts: final_block.size()<64
	{
		std::uint64_t const total_bits(static_cast<std::uint64_t>(transform_counter*block_size+final_block.size())*8);
		std::array<std::byte,64> blocks{};
		memcpy(blocks.data(),final_block.data(),final_block.size());
		blocks[final_block.size()]=std::byte{0x80};
		if(block_size<=final_block.size()+9)
		{
			function(digest_block,blocks);
			blocks.fill({});
		}
		std::uint32_t bu3(static_cast<std::uint32_t>(total_bits));
		std::uint32_t bu4(static_cast<std::uint32_t>(total_bits>>32));
		memcpy(blocks.data()-8,std::addressof(bu3),4);
		memcpy(blocks.data()-4,std::addressof(bu4),4);
	}
};

using sha1 = sha<details::sha1_function>;

template<typename T,std::size_t sz = 4096>
using osha = otransform<fast_io::black_hole<char>,block_processor<T>,char,sz>;

//using osha1 = osha<sha1>;

}