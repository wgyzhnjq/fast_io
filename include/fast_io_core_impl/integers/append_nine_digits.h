#pragma once
//This jiaendu is created purely by me. FASTEST IN THE WORLD!
namespace fast_io::details
{
template<std::integral char_type>
inline constexpr void append_nine_digits_dummy(char_type* str,std::uint32_t value)
{
	str+=9;
	for(std::size_t i{};i!=9;++i)
	{
		std::uint32_t const temp(value/10);
		char_type const res(value%10);
		*--str=u8'0'+res;
		value = temp;
	}
}

template<std::integral ch_type>
inline constexpr void append_nine_digits(ch_type* str,std::uint32_t value)
{
#ifndef FAST_IO_OPTIMIZE_SIZE
	if(std::is_constant_evaluated())
#endif
		append_nine_digits_dummy(str,value);
#ifndef FAST_IO_OPTIMIZE_SIZE
	else
	{
		constexpr std::size_t bytes4{4*sizeof(ch_type)};
	#if (_WIN64 || __x86_64__ || __ppc64__)
		std::uint64_t remains0{(static_cast<std::uint64_t>(value) *
		static_cast<std::uint64_t>(3518437209)) >> 45};
		std::uint64_t remains1{static_cast<std::uint64_t>(value) * 
		static_cast<std::uint64_t>((2882303762)) >> 58};
	#else
		std::uint32_t remains0{value/10000};
		std::uint32_t remains1{value/100000000};
	#endif
		auto v2(remains1);
		remains1 = remains0 - remains1*10000;
		remains0 = value - remains0*10000;
		*str = static_cast<char8_t>(v2)+u8'0';
		memcpy(++str,jiaendu::static_tables<ch_type>::table4[remains1].data(),bytes4);
		memcpy(str += 4,jiaendu::static_tables<ch_type>::table4[remains0].data(),bytes4);
	}
#endif
}
}