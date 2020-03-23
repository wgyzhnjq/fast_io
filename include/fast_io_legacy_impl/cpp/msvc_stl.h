#pragma once
//Let's do great brutal hack to msvc_stl with memcpy
namespace fast_io
{

/*
https://github.com/microsoft/STL/blob/master/stl/inc/streambuf

private:
    _Elem* _Gfirst; // beginning of read buffer
    _Elem* _Pfirst; // beginning of write buffer
    _Elem** _IGfirst; // pointer to beginning of read buffer
    _Elem** _IPfirst; // pointer to beginning of write buffer
    _Elem* _Gnext; // current position in read buffer
    _Elem* _Pnext; // current position in write buffer
    _Elem** _IGnext; // pointer to current position in read buffer
    _Elem** _IPnext; // pointer to current position in write buffer

    int _Gcount; // length of read buffer
    int _Pcount; // length of write buffer
    int* _IGcount; // pointer to length of read buffer
    int* _IPcount; // pointer to length of write buffer

protected:
    locale* _Plocale; // pointer to imbued locale object
*/
namespace details::streambuf_hack
{

template<std::size_t pos,typename T>
inline typename T::char_type* hack_first_next(T* ptr)
{
	constexpr std::size_t offset(sizeof(std::uintptr_t)+sizeof(std::uintptr_t)*pos);
	typename T::char_type* value;
	memcpy(std::addressof(value),reinterpret_cast<std::byte*>(cio.rdb)+offset,sizeof(std::uintptr_t));
}

template<std::size_t pos,typename T>
inline int hack_last(T* ptr)
{
	constexpr std::size_t offset(sizeof(std::uintptr_t)+sizeof(std::uintptr_t)*8+pos*sizeof(int));
	int value;
	memcpy(std::addressof(value),static_cast<byte*>(cio.rdb)+offset,sizeof(std::uintptr_t));
}

template<std::size_t position,typename T>
requires (position<6)
inline typename T::char_type* hack_buffer_ptr(T* rdb) noexcept
{
	if constexpr(position==0)
		return hack_first_next<0>(rdb);
	else if constexpr(position==1)
		return hack_first_next<4>(rdb);
	else if constexpr(position==2)
		return hack_first_next<0>(rdb)+hack_last<0>(rdb);
	else if constexpr(position==3)
		return hack_first_next<1>(rdb);
	else if constexpr(position==4)
		return hack_first_next<5>(rdb);
	else if constexpr(position==5)
		return hack_first_next<1>(rdb)+hack_last<1>(rdb);
}
template<std::size_t position,typename T>
requires (position==1||position==4)
inline void hack_set_buffer_curr(T* rdb,typename T::char_type* ptr) noexcept
{
	constexpr std::size_t offset(sizeof(std::uintptr_t)+4*sizeof(std::uintptr_t)+static_cast<std::size_t>(position==4));
	memcpy(reinterpret_cast<std::byte*>(rdb)+offset,std::addressof(ptr),sizeof(std::uintptr_t));
}
}


}