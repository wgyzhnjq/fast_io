#pragma once
#include <sys/mman.h>

namespace fast_io
{

template<std::size_t page_bytes_hint=2097152,typename T>
inline constexpr T* mmap_allocate(std::size_t n,T* hint=nullptr) noexcept
{
	if(std::is_constant_evaluated())
		return std::allocator_traits<std::allocator<T>>::allocate(std::allocator<T>(),hint,n);
	else
	{
#if defined(_POSIX_C_SOURCE)
//#if defined(__linux__)&&defined(__x86_64__)
//		if(ptr==nullptr)
//			std::abort();
		return reinterpret_cast<T*>(mmap(hint,sizeof(T)*n,PROT_READ|PROT_WRITE,
			MAP_PRIVATE | MAP_ANONYMOUS |MAP_HUGETLB,-1,0));
#else
#error "current not supported"
#endif
	}
}

template<std::size_t page_bytes_hint=2097152,typename T>
inline constexpr void mmap_deallocate(T* ptr,std::size_t n) noexcept
{
	if(std::is_constant_evaluated())
		std::allocator_traits<std::allocator<T>>::deallocate(std::allocator<T>(),ptr,sizeof(T)*n);
	else
	{
		if(ptr==nullptr)
			return;
#if defined(_POSIX_C_SOURCE)
		munmap(ptr,n);
#else
#error "current not supported"
#endif
	}
}
template<typename T,std::size_t page_bytes_hint=2097152>
class mmap_allocation_unique_ptr
{
public:
	T* internal_ptr{};
	constexpr T* get() noexcept
	{
		return internal_ptr;
	}
	constexpr T* get() const noexcept
	{
		return internal_ptr;
	}
	constexpr mmap_allocation_unique_ptr() = default;
	constexpr mmap_allocation_unique_ptr(std::size_t n,T* hint=nullptr):internal_ptr(mmap_allocate<page_bytes_hint>(n,hint)){}
	constexpr T* release() const noexcept
	{
		auto temp{internal_ptr};
		internal_ptr = nullptr;
		return temp;
	}
	mmap_allocation_unique_ptr(mmap_allocation_unique_ptr const&)=delete;
	mmap_allocation_unique_ptr& operator=(mmap_allocation_unique_ptr const&)=delete;
	constexpr mmap_allocation_unique_ptr(mmap_allocation_unique_ptr&& bmv) noexcept:internal_ptr(bmv.internal_ptr)
	{
		bmv.internal_ptr=nullptr;
	}
	constexpr mmap_allocation_unique_ptr& operator=(mmap_allocation_unique_ptr&& bmv) noexcept
	{
		if(bmv.internal_ptr==internal_ptr)[[unlikely]]
			return *this;
		mmap_deallocate<page_bytes_hint>(internal_ptr);
		internal_ptr=bmv.internal_ptr;
		bmv.internal_ptr=nullptr;
		return *this;
	}
	constexpr ~mmap_allocation_unique_ptr()
	{
		mmap_deallocate<page_bytes_hint>(internal_ptr);
	}
};

}