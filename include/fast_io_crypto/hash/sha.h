#pragma once
#include"sha1.h"
namespace fast_io
{

template<typename T>
struct sha
{
public:
	using function_type = T;
	typename T::digest_type digest_block = T::digest_initial_value;
	inline static constexpr std::size_t block_size = T::block_size;
	T function;
	std::size_t transform_counter{};
	void operator()(std::span<std::byte const,block_size> process_block)
	{
		function(digest_block,process_block);
		++transform_counter;
//		::debug_print("no crash\n");
	}
	void digest(std::span<std::byte const> final_block)//contracts: final_block.size()<64
	{
//		::debug_print("crash\n");
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

template<typename T>
inline constexpr std::size_t print_reserve_size(print_reserve_type_t<sha<T>>)
{
	return sizeof(typename T::digest_type)*8;
}

template<std::random_access_iterator caiter,typename T>
inline constexpr caiter print_reserve_define(print_reserve_type_t<sha<T>>,caiter iter,auto& i)
{
	for(auto e : i.digest_block)
	{
		fast_io::details::optimize_size::output_unsigned_dummy<8,16>(iter,e);
		iter+=8;
	}
	return iter;
}
}