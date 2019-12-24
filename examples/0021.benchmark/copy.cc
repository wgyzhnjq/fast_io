#include<cstring>
#include<algorithm>
#include<array>
#include<concepts>
#include<iterator>

auto copy_char8_t_array(char* out,std::array<char8_t,2> const& bits)
{
	return std::copy_n(bits.data(),bits.size(),out);
}


auto memcpy_char8_t_array(char* out,std::array<char8_t,2> const& bits)
{
	std::memcpy(out,bits.data(),bits.size());
	return bits.size();
}


auto copy_char_array(char* out,std::array<char,2> const& bits)
{
	return std::copy_n(bits.data(),bits.size(),out);
}


auto memcpy_char_array(char* out,std::array<char,2> const& bits)
{
	std::memcpy(out,bits.data(),bits.size());
	return bits.size();
}

auto copy_char_array_chars(char8_t* out,std::array<char8_t,2> const& bits)
{
	return std::copy_n(bits.data(),bits.size(),out);
}


auto memcpy_char_array_array_chars(char8_t* out,std::array<char8_t,2> const& bits)
{
	std::memcpy(out,bits.data(),bits.size());
	return bits.size();
}


auto copy_char_array(char8_t* out,std::array<char,2> const& bits)
{
	return std::copy_n(bits.data(),bits.size(),out);
}


auto memcpy_char_array(char8_t* out,std::array<char,2> const& bits)
{
	std::memcpy(out,bits.data(),bits.size());
	return bits.size();
}
template<std::input_iterator input_iter,std::input_iterator output_iter>
inline constexpr output_iter my_copy_n(input_iter first,std::size_t count,output_iter result)
{
	if constexpr
	(std::contiguous_iterator<input_iter>&&
	std::contiguous_iterator<output_iter>&&
	std::is_trivially_copyable_v<typename std::iterator_traits<input_iter>::value_type>&&
	std::is_trivially_copyable_v<typename std::iterator_traits<output_iter>::value_type>&&
	(std::same_as<typename std::iterator_traits<input_iter>::value_type,typename std::iterator_traits<output_iter>::value_type>||(std::integral<typename std::iterator_traits<input_iter>::value_type>&&
	std::integral<typename std::iterator_traits<output_iter>::value_type>&&sizeof(std::is_trivially_copyable_v<typename std::iterator_traits<input_iter>::value_type>)==sizeof(std::is_trivially_copyable_v<typename std::iterator_traits<output_iter>::value_type>))))
	{
		memmove(std::to_address(result),std::to_address(first),
			sizeof(typename std::iterator_traits<input_iter>::value_type)*count);
		return result+count;
	}
	return std::copy_n(first,count,result);
}

auto my_copy_char_array(char8_t* out,std::array<char,2> const& bits)
{
	return my_copy_n(bits.data(),bits.size(),out);
}
auto my_copy_char_array(char* out,std::array<char8_t,2> const& bits)
{
	return my_copy_n(bits.data(),bits.size(),out);
}


auto uninit_copy_char_array(char8_t* out,std::array<char,2> const& bits)
{
	return std::uninitialized_copy_n(bits.data(),bits.size(),out);
}
