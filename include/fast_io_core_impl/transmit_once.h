#pragma once

namespace fast_io
{

template<typename outputstm,typename inputstm>
requires output_stream<std::remove_cvref_t<outputstm>>&&input_stream<std::remove_cvref_t<inputstm>>
inline constexpr std::uintmax_t transmit_once(outputstm&& outp,inputstm&& inp)
{
	using output=std::remove_cvref_t<outputstm>;
	using input=std::remove_cvref_t<inputstm>;
	if constexpr(mutex_input_stream<input>)
	{
		typename input::lock_guard_type lg{mutex(inp)};
		decltype(auto) uh{unlocked_handle(inp)};
		return transmit_once(outp,uh);
	}
	else if constexpr(buffer_input_stream<input>)
	{
		std::uintmax_t bytes{};
		if(ibuffer_curr(inp)!=ibuffer_end(inp))[[unlikely]]
		{
			write(outp,ibuffer_curr(inp),ibuffer_end(inp));
			bytes += ibuffer_end(inp)-ibuffer_curr(inp);
		}
		if(!underflow(inp))[[unlikely]]
			return bytes;
		write(outp,ibuffer_curr(inp),ibuffer_end(inp));
		bytes += ibuffer_end(inp)-ibuffer_curr(inp);
		ibuffer_set_curr(inp,ibuffer_end(inp));
		return bytes;
	}
	else
	{
		using char_type = typename input::char_type;
		std::array<char_type,65536> buffer;
		auto it{read(inp,buffer.data(),buffer.data()+buffer.size())};
		write(outp,buffer.data(),it);
		return it-buffer.data();
	}
}

}