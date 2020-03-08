#pragma once
#include<concepts>
#include<type_traits>
#include<iterator>

namespace fast_io
{

template<std::movable T,typename Alloc = std::allocator<T>>
class vector
{
public:
	using value_type = T;
	using allocator_type = Alloc;
	using allocator_traits = std::allocator_traits<allocator_type>;
	using size_type = allocator_traits::size_type;
	using difference_type = allocator_traits::difference_type;
	using reference = value_type&;
	using const_reference = value_type const&;
	using pointer = allocator_traits::pointer;
	using const_pointer = allocator_traits::const_pointer;
	using iterator = pointer;
	using const_iterator = const_pointer;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;
	allocator_type storage_alloc{};
	pointer storage_begin{},storage_end{},storage_capacity{};
public:
	constexpr void clear() noexcept
	{
		if constexpr(is_trivially_destructible<value_type>)
			storage_end=storage_begin;
		else
			for(;storage_end!=storage_begin;--storage_end)
				allocator_traits::destroy(storage_alloc,storage_end-1);
	}
private:
	constexpr void storage_deallocate() noexcept
	{
		allocator_traits::deallocate(storage_alloc,storage_begin,storage_capacity-storage_begin);
	}
	constexpr void storage_reset() noexcept
	{
		storage_begin=storage_end=storage_capacity={};
	}
	constexpr void storage_clear_and_deallocate() noexcept
	{
		clear();
		storage_deallocate();
	}
	class storage_guard
	{
	public:
		vector<value_type,allocator_type>* ptr{};
		constexpr storage_guard(vector<value_type,allocator_type>* p) noexcept:ptr(p){}
		storage_guard(storage_guard const&)=delete;
		storage_guard const& operator=(storage_guard const&)=delete;
		constexpr void release() noexcept
		{
			ptr={};
		}
		constexpr ~storage_guard()
		{
			if(!ptr)
				return;
			ptr->storage_clear_and_deallocate();
		}
	};
public:
	constexpr vector() noexcept(noexcept(allocator_type()))=default;
	constexpr allocator_type get_allocator() const
	{
		return storage_alloc;
	}
	constexpr vector(vector const& para_other):vector()
	{
		
	}
	constexpr ~vector()
	{
		storage_clear_and_deallocate();
	}
	explicit constexpr vector(allocator_type const& para_alloc=allocator_type()) noexcept:
		storage_alloc(para_alloc){}
	constexpr vector(size_type para_count,const_reference para_value=value_type(),
		allocator_type const& para_alloc=allocator_type())
			requires(std::constructible_from<value_type,const_reference>):storage_alloc(para_alloc)
	{
		storage_guard gd(this);
		storage_capacity=(storage_end=storage_begin=allocator_traits::allocate(storage_alloc,para_count))+para_count;
		for(;storage_end!=storage_capacity;++storage_end)
			allocator_traits::construct(storage_alloc,storage_end,para_value);
		gd.release();
	}
};

}