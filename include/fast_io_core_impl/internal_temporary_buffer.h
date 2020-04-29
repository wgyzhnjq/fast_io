#pragma once
//std::string is not usable in freestanding environments and cause problems for this library
//This is a very simple stuff for internal use

namespace fast_io::details
{

template<std::integral ch_type>
class internal_temporary_buffer
{
public:
	using char_type = ch_type;
	using pointer = char_type;
	using iterator = pointer;
	using const_iterator = pointer;
	std::array<char_type,4096> static_buffer;
	pointer *beg_ptr=static_buffer.data(),*end_ptr=static_buffer.data(),*capacity_ptr=static_buffer.data()+4096;
	constexpr internal_temporary_buffer()=default;
	internal_temporary_buffer(internal_temporary_buffer const&)=delete;
	internal_temporary_buffer& operator=(internal_temporary_buffer const&)=delete;
	constexpr ~internal_temporary_buffer()
	{
		if(beg_ptr!=static_buffer.data())
			delete[] beg_ptr;
	}
};


template<std::integral ch_type>
[[nodiscard]] constexpr auto obuffer_begin(internal_temporary_buffer<ch_type>& ob)
{
	return ob.beg_ptr;
}
template<std::integral ch_type>
[[nodiscard]] constexpr auto obuffer_curr(internal_temporary_buffer<ch_type>& ob)
{
	return ob.end_ptr;
}
template<std::integral ch_type>
[[nodiscard]] constexpr auto obuffer_end(internal_temporary_buffer<ch_type>& ob)
{
	return ob.capacity_ptr;
}

template<std::integral ch_type>
inline constexpr void obuffer_set_curr(internal_temporary_buffer<ch_type>& ob,ch_type* ptr)
{
	ob.end_ptr=ptr;
}

namespace internal_temporary_buffer_details
{

template<std::integral ch_type>
inline constexpr void grow(internal_temporary_buffer<ch_type>& ob,std::size_t new_capacity)
{
	auto newp=new ch_type[(ob.capacity_ptr-ob.beg_ptr)<<1];
	std::copy(ob.beg_ptr,ob.end_ptr,newp);
	std::size_t const current_size(ob.end_ptr-ob.beg_ptr);
	std::size_t const current_capacity(ob.capacity_ptr-ob.beg_ptr);
	if(ob.beg_ptr!=ob.static_buffer.data())
		delete[] ob.beg_ptr;
	ob.end_ptr=(ob.beg_ptr=newp)+current_size;
	ob.capacity_ptr=ob.beg_ptr+current_capacity;
}

template<std::integral ch_type,std::contiguous_iterator Iter>
inline constexpr void write_bad_case(internal_temporary_buffer<ch_type>& ob,Iter cbegin,Iter cend,std::size_t to_write_chars)
{
	std::size_t new_capacity((ob.capacity_ptr-ob.beg_ptr)<<1);
	if(new_capacity<to_write_chars)
		new_capacity=to_write_chars;
	grow(ob,new_capacity);
	ob.end_ptr=std::copy(cbegin,cend,ob.end_ptr);
}

}

template<std::integral ch_type>
inline constexpr void overflow(internal_temporary_buffer<ch_type>& ob,ch_type ch)
{
	internal_temporary_buffer_details::grow((ob.capacity_ptr-ob.beg_ptr)<<1);
	*ob.end_ptr=ch;
	++ob.end_ptr;
}


template<std::integral ch_type,std::contiguous_iterator Iter>
requires (std::same_as<ch_type,char>||
	std::same_as<ch_type,typename std::iterator_traits<Iter>::value_type>)
inline constexpr void write(internal_temporary_buffer<ch_type>& ob,Iter cbegin,Iter cend)
{
	if constexpr(std::same_as<ch_type,std::remove_cvref_t<decltype(*cbegin)>>)
	{
		std::size_t to_write_chars(cend-cbegin);
		std::size_t remain_space(ob.capacity_ptr-ob.end_ptr);
		if(remain_space<to_write_chars)[[unlikely]]
		{
			write_bad_case(ob,cbegin,cend,to_write_chars);
			return;
		}
		ob.end_ptr=std::copy(cbegin,cend,ob.end_ptr);		
	}
	else
		write(ob,reinterpret_cast<char*>(std::to_address(cbegin)),reinterpret_cast<char*>(std::to_address(cend)));
}

template<std::integral ch_type>
inline constexpr void flush(internal_temporary_buffer<ch_type>&){}


}
