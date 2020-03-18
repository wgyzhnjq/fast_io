#pragma once

namespace fast_io::details::optimize_size
{

template<char8_t base=10,std::random_access_iterator Iter,std::unsigned_integral U>
inline constexpr std::size_t output_unsigned(Iter str,U value)
{
	using char_type = std::iter_value_t<Iter>;
	std::size_t const len{chars_len<base>(value)};
	str+=len;
	for(std::size_t i{};i!=len;++i)
	{
		U const temp(value/base);
		*--str=u8'0'+static_cast<char_type>(value%base);
		value = temp;
	}
	return len;
}

}