#pragma once
//https://www.ginac.de/~kreckel/fileno/

namespace fast_io::details::streambuf_hack
{

template<typename char_type,typename traits_type>
inline FILE* fp_hack(std::basic_filebuf<char_type,traits_type>* fbuf) noexcept
{
	FILE* fp{};
	// we can only do this or ubsanitizer will complain. Do not do down_cast
	memcpy(&fp,reinterpret_cast<std::byte*>(fbuf)+sizeof(std::basic_filebuf<char_type, traits_type>)-3*sizeof(std::uintptr_t),sizeof(fp));
	return fp;
}

#ifdef __cpp_rtti
template<typename T>
requires (std::same_as<T,std::basic_streambuf<typename T::char_type,typename T::traits_type>>
||std::derived_from<std::basic_filebuf<typename T::char_type,typename T::traits_type>,T>)
inline FILE* fp_hack(T* cio)
{
	using filebuf_type = std::basic_filebuf<typename T::char_type,typename T::traits_type>;
	if constexpr(std::derived_from<T,filebuf_type>)
		return fp_hack(cio);
	else
		return fp_hack(std::addressof(dynamic_cast<filebuf_type&>(*cio)));
}
#endif
}