#pragma once

#include<cstddef>
#include<cstdint>
#include<cstring>
#include<atomic>
#include<bit>
#include<array>

namespace fast_io
{

namespace details::allocation
{
#if defined(__WINNT__) || defined(_MSC_VER)
extern "C" void* __stdcall VirtualAlloc(void*,std::size_t,std::uint32_t,std::uint32_t) noexcept;
extern "C" int __stdcall VirtualFree(void*,std::size_t,std::uint32_t) noexcept;
#endif

template<typename T>
inline constexpr auto map_a_page(std::size_t allocate_bytes,T* hint=nullptr) noexcept
{
#if defined(__WINNT__) || defined(_MSC_VER)
	return reinterpret_cast<T*>(VirtualAlloc(nullptr,allocate_bytes,0x00001000|0x00002000,0x04));
#elif defined(__linux__)&&defined(__x86_64__)
	return reinterpret_cast<T*>(system_call<9,std::ptrdiff_t>(hint,allocate_bytes,PROT_READ|PROT_WRITE,
		MAP_PRIVATE | MAP_ANONYMOUS,0,0));
#elif defined(_POSIX_C_SOURCE)
	return reinterpret_cast<T*>(mmap(hint,allocate_bytes,PROT_READ|PROT_WRITE,
		MAP_PRIVATE | MAP_ANONYMOUS,0,0));
#else
#error "current not supported"
#endif

}

struct free_vector
{
	using value_type = std::byte*;
	using pointer = value_type*;
	pointer free_vector_begin{};
	pointer free_vector_end{};
	pointer free_vector_capacity{};
	constexpr bool empty() const noexcept
	{
		return free_vector_begin==free_vector_end;
	}
	constexpr std::size_t capacity() const noexcept
	{
		return free_vector_capacity-free_vector_begin;
	}
	constexpr std::size_t size() const noexcept
	{
		return free_vector_end-free_vector_begin;
	}
	void push_back(value_type const& value) noexcept
	{
		if(free_vector_end==free_vector_capacity)[[unlikely]]
		{
			std::size_t new_cap(capacity()<<1);
			if(!new_cap)
				new_cap=4096/sizeof(value_type);
			std::size_t current_size{size()};
			auto new_page{map_a_page(new_cap*sizeof(value_type),free_vector_begin)};
#if defined(__WINNT__) || defined(_MSC_VER)
			if(free_vector_begin)
			{
				memcpy(new_page,free_vector_begin,size()*sizeof(value_type));
				VirtualFree(free_vector_begin,capacity()*sizeof(value_type),0x00004000);
			}
#endif
			free_vector_end=(free_vector_begin=new_page)+current_size;
			free_vector_capacity=free_vector_begin+new_cap;
		}
		*free_vector_end=value;
		++free_vector_end;
	}
	constexpr void pop_back()
	{
		--free_vector_end;
	}
};


struct page_mapped
{
	std::byte* page_mapped_end{};
	std::byte* page_mapped_capacity{};
	std::size_t allocated_pages{};
};

struct spin_lock_guard
{
	std::atomic_flag* lock;
	spin_lock_guard(std::atomic_flag& e):lock(std::addressof(e))
	{
		while (lock->test_and_set(std::memory_order_acquire));
	}
	spin_lock_guard(spin_lock_guard const&) = delete;
	spin_lock_guard& operator=(spin_lock_guard const&) = delete;
	~spin_lock_guard()
	{
		lock->clear();
	}
};

struct bucket
{
	std::atomic_flag lock{};
	free_vector fvec;
	page_mapped pm;
};

inline std::byte* non_happy_buc_allocate(page_mapped& pm,std::size_t bytes)
{
	if(pm.allocated_pages==0)
	{
		pm.allocated_pages=4096;
		if(4096<bytes)
			pm.allocated_pages=bytes;
	}
	pm.page_mapped_capacity=(pm.page_mapped_end=map_a_page<std::byte>(pm.allocated_pages))+pm.allocated_pages;
	pm.allocated_pages<<=1;
	auto temp{pm.page_mapped_end};
	pm.page_mapped_end+=bytes;
	return temp;
}

inline std::byte* buc_allocate(bucket& buc,std::size_t bytes) noexcept
{
	spin_lock_guard lg{buc.lock};
	if(buc.fvec.empty())[[unlikely]]
	{
		if(buc.pm.page_mapped_end==buc.pm.page_mapped_capacity)[[unlikely]]
			return non_happy_buc_allocate(buc.pm,bytes);
		auto temp{buc.pm.page_mapped_end};
		buc.pm.page_mapped_end+=bytes;
		return temp;
	}
	return *--buc.fvec.free_vector_end;
}

inline void buc_deallocate(bucket& buc,std::byte* ptr) noexcept
{
	spin_lock_guard lg{buc.lock};
	buc.fvec.push_back(ptr);
}

inline constinit std::array<bucket,sizeof(std::byte*)*8-5> buckets;

inline std::byte* real_allocate(std::size_t sz)
{
	using namespace fast_io::details::allocation;
	return buc_allocate(fast_io::details::allocation::buckets[std::bit_width(sz>>5)],std::bit_ceil(sz));
}
}
}

inline void* operator new(std::size_t sz) noexcept
{
	return fast_io::details::allocation::real_allocate(sz);
}

inline void operator delete(void* ptr,std::size_t sz) noexcept
{
	if(ptr==nullptr)[[unlikely]]
		return;
	fast_io::details::allocation::buc_deallocate(fast_io::details::allocation::buckets[std::bit_width(sz>>5)],reinterpret_cast<std::byte*>(ptr));
}

inline void* operator new[](std::size_t sz) noexcept
{
	auto ptr{fast_io::details::allocation::real_allocate(sizeof(std::size_t)+sz)};
	std::memcpy(ptr,std::addressof(sz),sizeof(sz));
	return ptr+sizeof(std::size_t);
}

inline void operator delete[](void* ptr) noexcept
{
	if(ptr==nullptr)[[unlikely]]
		return;
	std::size_t bytes;
	auto real_ptr{reinterpret_cast<std::byte*>(ptr)-sizeof(std::size_t)};
	std::memcpy(std::addressof(bytes),real_ptr,sizeof(std::size_t));
	operator delete(real_ptr,bytes);
}