#pragma once

#ifdef __x86_64__
#include <wmmintrin.h>  // for intrinsics for AES-NI
#endif 

namespace fast_io::crypto
{

template<std::size_t N>
requires(N==16||N==32||N==48||N==64)
inline void fast_xor_assignment(std::span<std::byte,N> spa,std::span<std::byte,N> spb)
{
#ifdef __x86_64__
    __m128i a( _mm_loadu_si128(reinterpret_cast<__m128i*>(spa.data())));
    __m128i b( _mm_loadu_si128(reinterpret_cast<__m128i*>(spb.data())));
	__m128i ret(_mm_xor_si128(a, b));
	_mm_storeu_si128(reinterpret_cast<__m128i*>(spa.data()), ret);
	if constexpr(16<N)
	{
	}
	/*ret=_mm_xor_si128(reinterpret_cast<__m128i*>(spa.data()),reinterpret_cast<__m128i*>(spb.data()))
	if constexpr(256<N)
	{

	}
	if constexpr(384<N)
	{

	}*/
#else
    for()
#endif
}

}
