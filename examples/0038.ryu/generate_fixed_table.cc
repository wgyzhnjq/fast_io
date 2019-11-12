#include"../../include/fast_io.h"

inline constexpr std::uint32_t length_for_index(std::size_t idx) {
// [log_10(2^i)] = ((16 * i) * 78913L) >> 18 <-- floor
// +1 for ceil, +16 for mantissa, +8 to round up when dividing by 9
return (static_cast<std::uint32_t>(((16 * idx) * 1292913986ul) >> 32) + 25) / 9;
}

inline constexpr std::size_t length_for_index2(std::size_t idx)
{
	return (120*idx)/64+3;
}


inline constexpr auto generate_fixed_offset_table()
{
	inline constexpr std::size_t table_size(64);
	std::array<std::uint16_t,table_size> offset{};
	std::uint32_t sum(0);
	for(std::size_t i(0);i!=table_size;++i)
	{
		length_for_index(i)
	}
}

int main()
{
	
}
