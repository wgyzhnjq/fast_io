#pragma once

namespace fast_io
{

namespace details
{
class sha1_function
{
public:
	using digest_type = std::array<std::uint32_t,5>;
	using block_type = std::array<std::byte,64>;
	static inline constexpr digest_type digest_initial_value{0x67452301,0xefcdab89,0x98badcfe,0x10325476,0xc3d2e1f0};

	void operator()(span<std::uint32_t,5> digest,std::span<std::uint32_t,16>)
	{
		
	}
};
}

}