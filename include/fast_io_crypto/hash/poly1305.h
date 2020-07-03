#pragma once

#include <cstdio>

namespace fast_io
{

namespace details
{

inline constexpr void U64TO8(std::byte *p, std::uint64_t v) {
	p[0] = static_cast<std::byte>((v      ) & 0xff);
	p[1] = static_cast<std::byte>((v >>  8) & 0xff);
	p[2] = static_cast<std::byte>((v >> 16) & 0xff);
	p[3] = static_cast<std::byte>((v >> 24) & 0xff);
	p[4] = static_cast<std::byte>((v >> 32) & 0xff);
	p[5] = static_cast<std::byte>((v >> 40) & 0xff);
	p[6] = static_cast<std::byte>((v >> 48) & 0xff);
	p[7] = static_cast<std::byte>((v >> 56) & 0xff);
}
inline constexpr std::uint64_t U8TO64(std::byte const *p) {
	return
		(((std::uint64_t)(static_cast<std::uint64_t>(p[0]) & 0xff)      ) |
		(( std::uint64_t)(static_cast<std::uint64_t>(p[1]) & 0xff) <<  8) |
		(( std::uint64_t)(static_cast<std::uint64_t>(p[2]) & 0xff) << 16) |
		(( std::uint64_t)(static_cast<std::uint64_t>(p[3]) & 0xff) << 24) |
		(( std::uint64_t)(static_cast<std::uint64_t>(p[4]) & 0xff) << 32) |
		(( std::uint64_t)(static_cast<std::uint64_t>(p[5]) & 0xff) << 40) |
		(( std::uint64_t)(static_cast<std::uint64_t>(p[6]) & 0xff) << 48) |
		(( std::uint64_t)(static_cast<std::uint64_t>(p[7]) & 0xff) << 56));
}

#if defined(_MSC_VER)
	#include <intrin.h>

	struct uint128_t {
		unsigned long long lo;
		unsigned long long hi;
	};

	#define MUL(out, x, y) out.lo = _umul128((x), (y), &out.hi)
	#define ADD(out, in) { unsigned long long t = out.lo; out.lo += in.lo; out.hi += (out.lo < t) + in.hi; }
	#define ADDLO(out, in) { unsigned long long t = out.lo; out.lo += in; out.hi += (out.lo < t); }
	#define SHR(in, shift) (__shiftright128(in.lo, in.hi, (shift)))
	#define LO(in) (in.lo)

	#define POLY1305_NOINLINE __declspec(noinline)
#elif defined(__GNUC__)
	#if defined(__SIZEOF_INT128__)
		typedef unsigned __int128 uint128_t;
	#else
		typedef unsigned uint128_t __attribute__((mode(TI)));
	#endif

	inline constexpr void MUL(uint128_t& out, std::uint64_t x, std::uint64_t y) {out = static_cast<uint128_t>(x) * static_cast<uint128_t>(y);}
	inline constexpr void ADD(uint128_t& out, uint128_t x) {out += x;}
	inline constexpr void ADDLO(uint128_t& out, std::uint64_t x) {out += x;}
	inline constexpr auto SHR(uint128_t& in, auto shift) {return (std::uint64_t )(in >> (shift));}
	inline constexpr auto LO(uint128_t& in) {return (std::uint64_t)(in);}

#endif

}

struct poly1305
{
	inline static constexpr std::size_t block_size = 16;
	using key_type = std::array<std::byte,block_size>;
	key_type key;
	struct poly1305_state_internal_t {
		std::uint64_t r[3];
		std::uint64_t h[3];
		std::uint64_t pad[2];
		std::size_t leftover;
		unsigned char buffer[block_size];
		unsigned char final;
	} internal_state;
	poly1305(std::span<std::byte const> init_key)
	{
		key_type key = {};
		if (init_key.size()>32) {
			memcpy(key.data(),init_key.data(),32);
		} else {
			memcpy(key.data(),init_key.data(),init_key.size());
		}
		poly1305_state_internal_t *st = &internal_state;
		unsigned long long t0,t1;

		/* r &= 0xffffffc0ffffffc0ffffffc0fffffff */
		t0 = details::U8TO64(&key[0]);
		t1 = details::U8TO64(&key[8]);

		st->r[0] = ( t0                    ) & 0xffc0fffffff;
		st->r[1] = ((t0 >> 44) | (t1 << 20)) & 0xfffffc0ffff;
		st->r[2] = ((t1 >> 24)             ) & 0x00ffffffc0f;

		/* h = 0 */
		st->h[0] = 0;
		st->h[1] = 0;
		st->h[2] = 0;

		/* save pad for later */
		st->pad[0] = details::U8TO64(&key[16]);
		st->pad[1] = details::U8TO64(&key[24]);

		st->leftover = 0;
		st->final = 0;
	}
	poly1305(std::string_view key):poly1305(std::as_bytes(std::span{key.data(),key.size()})){}
	std::size_t block_init(std::span<std::byte,block_size> sp)
	{
		return 0;
	}
	void operator()(std::span<std::byte const> process_block)
	{
		if(process_block.size()!=block_size)
		{
			std::printf("size=%lld\n",process_block.size());
			throw std::runtime_error("block size mismatch");
		}
		poly1305_state_internal_t *st = &internal_state;
		const unsigned long long hibit = (st->final) ? 0 : ((unsigned long long)1 << 40); /* 1 << 128 */
		unsigned long long r0,r1,r2;
		unsigned long long s1,s2;
		unsigned long long h0,h1,h2;
		unsigned long long c;
		uint128_t d0,d1,d2,d;

		r0 = st->r[0];
		r1 = st->r[1];
		r2 = st->r[2];

		h0 = st->h[0];
		h1 = st->h[1];
		h2 = st->h[2];

		s1 = r1 * (5 << 2);
		s2 = r2 * (5 << 2);

		using namespace details;

		std::byte const * m(process_block.data());
		{
			unsigned long long t0,t1;

			/* h += m[i] */
			t0 = U8TO64(&m[0]);
			t1 = U8TO64(&m[8]);

			h0 += (( t0                    ) & 0xfffffffffff);
			h1 += (((t0 >> 44) | (t1 << 20)) & 0xfffffffffff);
			h2 += (((t1 >> 24)             ) & 0x3ffffffffff) | hibit;

			/* h *= r */
			MUL(d0, h0, r0); MUL(d, h1, s2); ADD(d0, d); MUL(d, h2, s1); ADD(d0, d);
			MUL(d1, h0, r1); MUL(d, h1, r0); ADD(d1, d); MUL(d, h2, s2); ADD(d1, d);
			MUL(d2, h0, r2); MUL(d, h1, r1); ADD(d2, d); MUL(d, h2, r0); ADD(d2, d);

			/* (partial) h %= p */
						c = SHR(d0, 44); h0 = LO(d0) & 0xfffffffffff;
			ADDLO(d1, c); c = SHR(d1, 44); h1 = LO(d1) & 0xfffffffffff;
			ADDLO(d2, c); c = SHR(d2, 42); h2 = LO(d2) & 0x3ffffffffff;
			h0  += c * 5; c = (h0 >> 44);  h0 =    h0  & 0xfffffffffff;
			h1  += c;
		}

		st->h[0] = h0;
		st->h[1] = h1;
		st->h[2] = h2;
		//this->operator(process_block);
	}
	void operator()(std::span<std::byte const,block_size> process_blocks)
	{
		throw std::runtime_error("not implemented");//this->operator(process_blocks);
	}
	void digest(std::span<std::byte const> final_block)
	{
		poly1305_state_internal_t *st = &internal_state;
		std::uint64_t h0,h1,h2,c;
		std::uint64_t g0,g1,g2;
		std::uint64_t t0,t1;

		// pad zero and process last block
		std::array<std::byte, block_size> block_padded = {};
		memcpy(block_padded.data(), final_block.data(), final_block.size());
		(*this)(std::span<std::byte const,block_size>(block_padded));

		/* fully carry h */
		h0 = st->h[0];
		h1 = st->h[1];
		h2 = st->h[2];

					c = (h1 >> 44); h1 &= 0xfffffffffff;
		h2 += c;     c = (h2 >> 42); h2 &= 0x3ffffffffff;
		h0 += c * 5; c = (h0 >> 44); h0 &= 0xfffffffffff;
		h1 += c;     c = (h1 >> 44); h1 &= 0xfffffffffff;
		h2 += c;     c = (h2 >> 42); h2 &= 0x3ffffffffff;
		h0 += c * 5; c = (h0 >> 44); h0 &= 0xfffffffffff;
		h1 += c;

		/* compute h + -p */
		g0 = h0 + 5; c = (g0 >> 44); g0 &= 0xfffffffffff;
		g1 = h1 + c; c = (g1 >> 44); g1 &= 0xfffffffffff;
		g2 = h2 + c - ((std::uint64_t)1 << 42);

		/* select h if h < p, or h + -p if h >= p */
		c = (g2 >> ((sizeof(std::uint64_t) * 8) - 1)) - 1;
		g0 &= c;
		g1 &= c;
		g2 &= c;
		c = ~c;
		h0 = (h0 & c) | g0;
		h1 = (h1 & c) | g1;
		h2 = (h2 & c) | g2;

		/* h = (h + pad) */
		t0 = st->pad[0];
		t1 = st->pad[1];

		h0 += (( t0                    ) & 0xfffffffffff)    ; c = (h0 >> 44); h0 &= 0xfffffffffff;
		h1 += (((t0 >> 44) | (t1 << 20)) & 0xfffffffffff) + c; c = (h1 >> 44); h1 &= 0xfffffffffff;
		h2 += (((t1 >> 24)             ) & 0x3ffffffffff) + c;                 h2 &= 0x3ffffffffff;

		/* mac = h % (2^128) */
		h0 = ((h0      ) | (h1 << 44));
		h1 = ((h1 >> 20) | (h2 << 24));

		// U64TO8(&mac[0], h0);
		// U64TO8(&mac[8], h1);
	}
};


inline constexpr std::size_t print_reserve_size(print_reserve_type_t<poly1305>)
{
	return 1;
}

template<std::random_access_iterator caiter>
inline constexpr caiter print_reserve_define(print_reserve_type_t<poly1305>,caiter iter,auto& i)
{
	return iter + 1;
}


}