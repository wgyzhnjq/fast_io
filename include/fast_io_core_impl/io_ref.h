#pragma once

namespace fast_io
{

template<stream T>
class io_ref
{
	T* ptr;
public:
	using char_type = typename T::char_type;
	constexpr io_ref(T& t): ptr(std::addressof(t)){}
	constexpr T& operator*() const
	{
		return *ptr;
	}
	constexpr T& operator*()
	{
		return *ptr;
	}
	constexpr T* operator->() const
	{
		return ptr;
	}
	constexpr T* operator->()
	{
		return ptr;
	}
};

template<stream T>
inline constexpr T* to_address(io_ref<T>& ref)
{
	return ref.operator->();
}

template<stream T>
inline constexpr T const* to_address(io_ref<T> const& ref)
{
	return ref.operator->();
}

template<stream srm>
io_ref(srm&) -> io_ref<srm>;

template<input_stream input,std::contiguous_iterator Iter>
inline constexpr Iter read(io_ref<input>& in,Iter begin,Iter end)
{
	return read(*in,begin,end);
}

template<output_stream output,std::contiguous_iterator Iter>
inline constexpr auto write(io_ref<output>& out,Iter begin,Iter end)
{
	return write(*out,begin,end);
}

template<output_stream output>
inline constexpr void flush(io_ref<output>& out)
{
	flush(*out);
}

template<mutex_stream T>
inline constexpr decltype(auto) mutex(io_ref<T>& t)
{
	return mutex(*t);
}


template<zero_copy_input_stream in>
inline constexpr decltype(auto) zero_copy_in_handle(io_ref<in>& t)
{
	return zero_copy_in_handle(*t);
}

template<buffer_input_stream in>
inline constexpr decltype(auto) begin(io_ref<in>& t)
{
	return begin(*t);
}

template<buffer_input_stream in>
inline constexpr decltype(auto) end(io_ref<in>& t)
{
	return end(*t);
}

template<buffer_input_stream in>
inline constexpr decltype(auto) operator++(io_ref<in>& t)
{
	return operator++(*t);
}

template<buffer_input_stream in>
inline constexpr decltype(auto) iflush(io_ref<in>& t)
{
	return iflush(*t);
}

template<character_output_stream output>
inline constexpr void put(io_ref<output>& out,typename output::char_type ch)
{
	put(*out,ch);
}

template<zero_copy_output_stream out>
inline constexpr decltype(auto) zero_copy_out_handle(io_ref<out>& t)
{
	return zero_copy_out_handle(*t);
}


template<buffer_output_stream out>
inline constexpr decltype(auto) oreserve(io_ref<out>& t)
{
	return oreserve(*t);
}

template<buffer_output_stream out>
inline constexpr decltype(auto) orelease(io_ref<out>& t)
{
	orelease(*t);
}


}
