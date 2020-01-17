#pragma once

namespace fast_io::crypto
{

template<std::size_t N>
requires(N==128||N==256||N==384||N==512)
inline void fast_xor_assignment(std::span<std::byte,N> spa,std::span<std::byte,N> spb)
{
	__m128i ret(_mm_xor_su128(reinterpret_cast<__m128u*>(spa.data()),reinterpret_cast<__m128u*>(spb.data())));
	if constexpr(128<N)
	{
		_mm_store_su128(reinterpret_cast<__m128u*>(spa.data()));
		return;
	}
	ret=_mm_xor_su128(reinterpret_cast<__m128u*>(spa.data()),reinterpret_cast<__m128u*>(spb.data()))
	if constexpr(256<N)
	{

	}
	if constexpr(384<N)
	{

	}
}

}