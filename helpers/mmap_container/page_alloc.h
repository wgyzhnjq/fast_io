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
extern "C" void* __stdcall VirtualAlloc(void*,std::size_t,std::uint32_t,std::uint32_t);
extern "C" int __stdcall VirtualFree(void*,std::size_t,std::uint32_t);
#endif

template<typename T>
inline constexpr auto map_a_page(std::size_t allocate_bytes,T* hint=nullptr)
{
#if defined(__WINNT__) || defined(_MSC_VER)
	std::uint32_t flags{0x00001000|0x00002000};//MEM_COMMIT|MEM_RESERVE
	if(1048576<=allocate_bytes)//large pages probably
		flags|=0x20000000;
	return reinterpret_cast<T*>(VirtualAlloc(nullptr,allocate_bytes,flags,0x04));
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
		pm.allocated_pages=4096;
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

inline constinit std::array<bucket,64> buckets;
}
}

inline void* operator new(std::size_t sz) noexcept
{
	sz>>=5;
	using namespace fast_io::details::allocation;
	return buc_allocate(fast_io::details::allocation::buckets[std::bit_width(sz)],std::bit_ceil(sz));
}

inline void operator delete(void* ptr,std::size_t sz) noexcept
{
	sz>>=5;
	return buc_deallocate(fast_io::details::allocation::buckets[std::bit_width(sz)],reinterpret_cast<std::byte*>(ptr));
}