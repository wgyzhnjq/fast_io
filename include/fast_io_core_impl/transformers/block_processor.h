#pragma once

namespace fast_io
{

template<typename Func>
class block_processor
{
public:
	using function_type = Func;
	function_type function;
	inline static constexpr std::size_t block_size = function_type::block_size;
	template<output_stream output, std::contiguous_iterator Iter>
	requires (sizeof(typename output::char_type)==1)
	Iter write_proxy(output& out,Iter b, Iter e)
	{
		std::span be(std::as_bytes(std::span(b,e)));
		for(;block_size<=be.size();)
		{
			if constexpr(std::same_as<std::result_of_t<function_type>,void>)
				function(std::span<std::byte,block_size>(be));
			else
			{
				auto ret(function(std::span<std::byte,block_size>(be)));
				write(out,ret);
			}
			be=be.subspan(block_size);
		}
		return e - be.size() / sizeof(*b);
	}
	template<output_stream output, std::contiguous_iterator Iter>
	requires (sizeof(typename output::char_type)==1&&requires()
	{
		function.digest(std::span<std::byte,block_size>{});
	})
	void flush_proxy(output& out,Iter begin, Iter end)
	{
		begin = write_proxy(out, begin, end);
		auto plain_text(std::as_writable_bytes(std::span<char>(begin,end)));
		std::span be(std::as_bytes(std::span(begin,end)));
		if constexpr(requires()
		{
			{function.digest()}->std::same_as<void>;
		})
			function.digest(std::span<std::byte,block_size>(be));
		else
		{
			auto ret(function.digest(std::span<std::byte,block_size>(be)));
			write(out,ret);
		}
	}
	template<input_stream input, std::contiguous_iterator Iter>
	requires (sizeof(typename input::char_type)==1)
	Iter read_proxy(input& in,Iter b, Iter e)
	{
		auto be(std::as_writable_bytes(std::span<std::remove_reference_t<decltype(*b)>>(b, e)));
		for(;block_size<=be.size();)
		{
			if (!ireserve(in, block_size))
				break;
			auto ret(function(std::span<std::byte, block_size>(reinterpret_cast<std::byte*>(begin(in)))));
			details::my_copy_n(ret.data(), block_size, be.data());
			be=be.subspan(block_size);
		}
		return e - be.size() / sizeof(*b);
	}
};

}