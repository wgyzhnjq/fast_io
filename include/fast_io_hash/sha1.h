#pragma once

#ifdef __x86_64__
//requires -maes -msse2
#include <immintrin.h>  // for intrinsics for sha1 function

namespace fast_io
{

namespace details
{
class sha1_function
{
public:
	using digest_type = std::array<std::uint32_t,5>;
	static inline constexpr digest_type digest_initial_value{0x67452301,0xefcdab89,0x98badcfe,0x10325476,0xc3d2e1f0};
	static inline constexpr std::size_t block_size{64};
	void operator()(std::span<std::uint32_t,5> state,std::span<std::byte,64> blocks)
	{
//https://stackoverflow.com/questions/21107350/how-can-i-access-sha-intrinsic
		__m128i ABCD, ABCD_SAVE, E0, E0_SAVE, E1;
		__m128i MASK, MSG0, MSG1, MSG2, MSG3;
//Clearly it is a violation of strict aliasing rule.
//However, the library will mark them as mayalias. We can safely use them.

		// Load initial values
		ABCD = _mm_loadu_si128((__m128i*) state.data());
		E0 = _mm_set_epi32(state[4], 0, 0, 0);
		ABCD = _mm_shuffle_epi32(ABCD, 0x1B);
		MASK = _mm_set_epi64x(0x0001020304050607ULL, 0x08090a0b0c0d0e0fULL);

		// Save current hash
		ABCD_SAVE = ABCD;
		E0_SAVE = E0;

		// Rounds 0-3
		MSG0 = _mm_loadu_si128((__m128i*) blocks.data());
		MSG0 = _mm_shuffle_epi8(MSG0, MASK);
		E0 = _mm_add_epi32(E0, MSG0);
		E1 = ABCD;
		ABCD = _mm_sha1rnds4_epu32(ABCD, E0, 0);

		// Rounds 4-7
		MSG1 = _mm_loadu_si128((__m128i*) (blocks.data()+4));
		MSG1 = _mm_shuffle_epi8(MSG1, MASK);
		E1 = _mm_sha1nexte_epu32(E1, MSG1);
		E0 = ABCD;
		ABCD = _mm_sha1rnds4_epu32(ABCD, E1, 0);
		MSG0 = _mm_sha1msg1_epu32(MSG0, MSG1);

		// Rounds 8-11
		MSG2 = _mm_loadu_si128((__m128i*) (blocks.data()+8));
		MSG2 = _mm_shuffle_epi8(MSG2, MASK);
		E0 = _mm_sha1nexte_epu32(E0, MSG2);
		E1 = ABCD;
		ABCD = _mm_sha1rnds4_epu32(ABCD, E0, 0);
		MSG1 = _mm_sha1msg1_epu32(MSG1, MSG2);
		MSG0 = _mm_xor_si128(MSG0, MSG2);

		// Rounds 12-15
		MSG3 = _mm_loadu_si128((__m128i*) (blocks.data()+12));
		MSG3 = _mm_shuffle_epi8(MSG3, MASK);
		E1 = _mm_sha1nexte_epu32(E1, MSG3);
		E0 = ABCD;
		MSG0 = _mm_sha1msg2_epu32(MSG0, MSG3);
		ABCD = _mm_sha1rnds4_epu32(ABCD, E1, 0);
		MSG2 = _mm_sha1msg1_epu32(MSG2, MSG3);
		MSG1 = _mm_xor_si128(MSG1, MSG3);

		// Rounds 16-19
		E0 = _mm_sha1nexte_epu32(E0, MSG0);
		E1 = ABCD;
		MSG1 = _mm_sha1msg2_epu32(MSG1, MSG0);
		ABCD = _mm_sha1rnds4_epu32(ABCD, E0, 0);
		MSG3 = _mm_sha1msg1_epu32(MSG3, MSG0);
		MSG2 = _mm_xor_si128(MSG2, MSG0);

		// Rounds 20-23
		E1 = _mm_sha1nexte_epu32(E1, MSG1);
		E0 = ABCD;
		MSG2 = _mm_sha1msg2_epu32(MSG2, MSG1);
		ABCD = _mm_sha1rnds4_epu32(ABCD, E1, 1);
		MSG0 = _mm_sha1msg1_epu32(MSG0, MSG1);
		MSG3 = _mm_xor_si128(MSG3, MSG1);

		// Rounds 24-27
		E0 = _mm_sha1nexte_epu32(E0, MSG2);
		E1 = ABCD;
		MSG3 = _mm_sha1msg2_epu32(MSG3, MSG2);
		ABCD = _mm_sha1rnds4_epu32(ABCD, E0, 1);
		MSG1 = _mm_sha1msg1_epu32(MSG1, MSG2);
		MSG0 = _mm_xor_si128(MSG0, MSG2);

		// Rounds 28-31
		E1 = _mm_sha1nexte_epu32(E1, MSG3);
		E0 = ABCD;
		MSG0 = _mm_sha1msg2_epu32(MSG0, MSG3);
		ABCD = _mm_sha1rnds4_epu32(ABCD, E1, 1);
		MSG2 = _mm_sha1msg1_epu32(MSG2, MSG3);
		MSG1 = _mm_xor_si128(MSG1, MSG3);

		// Rounds 32-35
		E0 = _mm_sha1nexte_epu32(E0, MSG0);
		E1 = ABCD;
		MSG1 = _mm_sha1msg2_epu32(MSG1, MSG0);
		ABCD = _mm_sha1rnds4_epu32(ABCD, E0, 1);
		MSG3 = _mm_sha1msg1_epu32(MSG3, MSG0);
		MSG2 = _mm_xor_si128(MSG2, MSG0);

		// Rounds 36-39
		E1 = _mm_sha1nexte_epu32(E1, MSG1);
		E0 = ABCD;
		MSG2 = _mm_sha1msg2_epu32(MSG2, MSG1);
		ABCD = _mm_sha1rnds4_epu32(ABCD, E1, 1);
		MSG0 = _mm_sha1msg1_epu32(MSG0, MSG1);
		MSG3 = _mm_xor_si128(MSG3, MSG1);

		// Rounds 40-43
		E0 = _mm_sha1nexte_epu32(E0, MSG2);
		E1 = ABCD;
		MSG3 = _mm_sha1msg2_epu32(MSG3, MSG2);
		ABCD = _mm_sha1rnds4_epu32(ABCD, E0, 2);
		MSG1 = _mm_sha1msg1_epu32(MSG1, MSG2);
		MSG0 = _mm_xor_si128(MSG0, MSG2);

		// Rounds 44-47
		E1 = _mm_sha1nexte_epu32(E1, MSG3);
		E0 = ABCD;
		MSG0 = _mm_sha1msg2_epu32(MSG0, MSG3);
		ABCD = _mm_sha1rnds4_epu32(ABCD, E1, 2);
		MSG2 = _mm_sha1msg1_epu32(MSG2, MSG3);
		MSG1 = _mm_xor_si128(MSG1, MSG3);

		// Rounds 48-51
		E0 = _mm_sha1nexte_epu32(E0, MSG0);
		E1 = ABCD;
		MSG1 = _mm_sha1msg2_epu32(MSG1, MSG0);
		ABCD = _mm_sha1rnds4_epu32(ABCD, E0, 2);
		MSG3 = _mm_sha1msg1_epu32(MSG3, MSG0);
		MSG2 = _mm_xor_si128(MSG2, MSG0);

		// Rounds 52-55
		E1 = _mm_sha1nexte_epu32(E1, MSG1);
		E0 = ABCD;
		MSG2 = _mm_sha1msg2_epu32(MSG2, MSG1);
		ABCD = _mm_sha1rnds4_epu32(ABCD, E1, 2);
		MSG0 = _mm_sha1msg1_epu32(MSG0, MSG1);
		MSG3 = _mm_xor_si128(MSG3, MSG1);

		// Rounds 56-59
		E0 = _mm_sha1nexte_epu32(E0, MSG2);
		E1 = ABCD;
		MSG3 = _mm_sha1msg2_epu32(MSG3, MSG2);
		ABCD = _mm_sha1rnds4_epu32(ABCD, E0, 2);
		MSG1 = _mm_sha1msg1_epu32(MSG1, MSG2);
		MSG0 = _mm_xor_si128(MSG0, MSG2);

		// Rounds 60-63
		E1 = _mm_sha1nexte_epu32(E1, MSG3);
		E0 = ABCD;
		MSG0 = _mm_sha1msg2_epu32(MSG0, MSG3);
		ABCD = _mm_sha1rnds4_epu32(ABCD, E1, 3);
		MSG2 = _mm_sha1msg1_epu32(MSG2, MSG3);
		MSG1 = _mm_xor_si128(MSG1, MSG3);

		// Rounds 64-67
		E0 = _mm_sha1nexte_epu32(E0, MSG0);
		E1 = ABCD;
		MSG1 = _mm_sha1msg2_epu32(MSG1, MSG0);
		ABCD = _mm_sha1rnds4_epu32(ABCD, E0, 3);
		MSG3 = _mm_sha1msg1_epu32(MSG3, MSG0);
		MSG2 = _mm_xor_si128(MSG2, MSG0);

		// Rounds 68-71
		E1 = _mm_sha1nexte_epu32(E1, MSG1);
		E0 = ABCD;
		MSG2 = _mm_sha1msg2_epu32(MSG2, MSG1);
		ABCD = _mm_sha1rnds4_epu32(ABCD, E1, 3);
		MSG3 = _mm_xor_si128(MSG3, MSG1);

		// Rounds 72-75
		E0 = _mm_sha1nexte_epu32(E0, MSG2);
		E1 = ABCD;
		MSG3 = _mm_sha1msg2_epu32(MSG3, MSG2);
		ABCD = _mm_sha1rnds4_epu32(ABCD, E0, 3);

		// Rounds 76-79
		E1 = _mm_sha1nexte_epu32(E1, MSG3);
		E0 = ABCD;
		ABCD = _mm_sha1rnds4_epu32(ABCD, E1, 3);

		// Add values back to state
		E0 = _mm_sha1nexte_epu32(E0, E0_SAVE);
		ABCD = _mm_add_epi32(ABCD, ABCD_SAVE);

		// Save state
		ABCD = _mm_shuffle_epi32(ABCD, 0x1B);
		_mm_storeu_si128((__m128i*) state.data(), ABCD);
		state[4] = _mm_extract_epi32(E0, 3);

		
	}
};
}

}

#endif