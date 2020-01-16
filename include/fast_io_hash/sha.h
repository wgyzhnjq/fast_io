#pragma once

namespace fast_io
{

template<typename T>
struct sha
{
public:
	using char_type = char;
	using function_type = T;
	typename T::digest_type digest = T::digest_initial_value;
	typename T::block_type block{};
	T function;
	std::size_t transform_counter{};
	template<output_stream output, std::contiguous_iterator Iter>
	inline constexpr Iter write_proxy(output&, Iter begin, Iter end)
	{
		std::span be(std::as_bytes(std::span(begin,end)));
		for(;block.size()<=be.size();++transform_counter)
		{
			function(std::span(digest),std::span<std::byte,64>(block));
			be=subspan(block.size());
		}
		return e - be.size() / sizeof(*b);
	}
	template<input_stream input,std::contiguous_iterator Iter>
	inline constexpr Iter read_proxy(input& input,Iter begin,Iter end)
	{
		
	}
};


}