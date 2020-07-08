#pragma once

namespace fast_io
{
namespace details
{
//usable unique_ptr in constexpr context
template<typename T>
class temp_unique_arr_ptr
{
public:
	T* ptr{};
	constexpr temp_unique_arr_ptr()=default;
	FIO_DYN_CONSTEXPR temp_unique_arr_ptr(std::size_t size):ptr(new T[size]){}
	temp_unique_arr_ptr(temp_unique_arr_ptr const&)=delete;
	temp_unique_arr_ptr& operator=(temp_unique_arr_ptr const&)=delete;
	constexpr temp_unique_arr_ptr(temp_unique_arr_ptr&& bmv) noexcept:ptr(bmv.ptr)
	{
		bmv.ptr=nullptr;
	}
	constexpr T* data() noexcept
	{
		return ptr;
	}
	constexpr T* data() const noexcept
	{
		return ptr;
	}
	constexpr T* get() noexcept
	{
		return ptr;
	}
	constexpr T* get() const noexcept
	{
		return ptr;
	}
	FIO_DYN_CONSTEXPR void reset(T* nptr) noexcept
	{
		if(ptr)[[likely]]
			delete[] ptr;
		ptr=nptr;
	}
	FIO_DYN_CONSTEXPR temp_unique_arr_ptr& operator=(temp_unique_arr_ptr&& bmv) noexcept
	{
		if(bmv.ptr==ptr)
			return *this;
		delete[] ptr;
		ptr=bmv.ptr;
		bmv.ptr=nullptr;
		return *this;
	}
	FIO_DYN_CONSTEXPR ~temp_unique_arr_ptr()
	{
		delete[] ptr;
	}
};


template<typename T>
class temp_unique_allocator_ptr
{
public:
	std::size_t size;
	T* ptr;
	constexpr temp_unique_allocator_ptr():size(),ptr(){}
	FIO_DYN_CONSTEXPR temp_unique_allocator_ptr(std::size_t sz):size(sz)
	{
		std::allocator<T> alloc;
		ptr=std::allocator_traits<std::allocator<T>>::allocate(alloc,size);
	}
	temp_unique_allocator_ptr(temp_unique_allocator_ptr const&)=delete;
	temp_unique_allocator_ptr& operator=(temp_unique_allocator_ptr const&)=delete;
	constexpr temp_unique_allocator_ptr(temp_unique_allocator_ptr&& bmv) noexcept:size(bmv.size),ptr(bmv.ptr)
	{
		bmv.size={};
		bmv.ptr=nullptr;
	}
	constexpr T* data() noexcept
	{
		return ptr;
	}
	constexpr T* data() const noexcept
	{
		return ptr;
	}
	constexpr T* get() noexcept
	{
		return ptr;
	}
	constexpr T* get() const noexcept
	{
		return ptr;
	}
	FIO_DYN_CONSTEXPR void reset(T* nptr,std::size_t sz) noexcept
	{
		if(ptr)[[likely]]
		{
			std::allocator<T> alloc;
			std::allocator_traits<std::allocator<T>>::deallocate(alloc,ptr,size);
		}
		size=sz;
		ptr=nptr;
	}
	FIO_DYN_CONSTEXPR temp_unique_allocator_ptr& operator=(temp_unique_allocator_ptr&& bmv) noexcept
	{
		if(bmv.ptr==ptr)
			return *this;
		if(ptr)[[likely]]
		{
			std::allocator<T> alloc;
			std::allocator_traits<std::allocator<T>>::deallocate(alloc,ptr,size);
		}
		size=bmv.size;
		ptr=bmv.ptr;
		bmv.size={};
		bmv.ptr=nullptr;
		return *this;
	}
	FIO_DYN_CONSTEXPR ~temp_unique_allocator_ptr()
	{
		if(ptr)[[likely]]
		{
			std::allocator<T> alloc;
			std::allocator_traits<std::allocator<T>>::deallocate(alloc,ptr,size);
		}
	}
};
}
}