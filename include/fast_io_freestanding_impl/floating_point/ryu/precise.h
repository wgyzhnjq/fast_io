#pragma once

namespace fast_io::details::ryu
{
template<std::floating_point floating_type,std::size_t buffer_size>
inline constexpr void precise(typename floating_traits<floating_type>::mantissa_type bits,stack_arbitary_int<char8_t,buffer_size>& output)
{
	using floating_trait = floating_traits<floating_type>;
	using mantissa_type = typename floating_trait::mantissa_type;
	using exponent_type = typename floating_trait::exponent_type;
	using signed_exponent_type = std::make_signed_t<exponent_type>;
	mantissa_type const mantissa(bits & ((static_cast<mantissa_type>(1u) << floating_trait::mantissa_bits) - 1u));
	exponent_type const exponent(static_cast<exponent_type>(((bits >> floating_trait::mantissa_bits) & floating_trait::exponent_max)));
	auto const r2(init_rep<F>(mantissa,static_cast<signed_exponent_type>(exponent)));
	auto r2m{r2.m};
	auto r2e{r2.e};
}

}