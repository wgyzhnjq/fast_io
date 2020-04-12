#pragma once
#include<cstddef>
#include<cstdlib>
#include"mmap_allocate.h"

namespace fast_io
{

struct default_init_t
{
	explicit constexpr default_init_t() = default;
};

inline constexpr default_init_t default_init{};

template<std::movable T,std::size_t N,std::size_t page_bytes_hint=2097152>
class mmap_stack_vector
{
public:
	using value_type = T;
	using difference_type = std::ptrdiff_t;
	using reference = T&;
	using const_reference = T const&;
	using pointer = T*;
	using const_pointer = T const*;
	using iterator = pointer;
	using const_iterator = const_pointer;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;
	pointer begin_ptr{},end_ptr{},capacity_ptr{};
	std::aligned_storage<sizeof(value_type),alignof(value_type)> static_storage[N];
	constexpr mmap_stack_vector() noexcept
	{
		if(std::is_constant_evaluted())
			capacity_ptr=(end_ptr=begin_ptr=std::allocate_traits<std::allocator<value_type>>::allocate(std::allocator<value_type>(),N))+N;
		else
		{
			capacity_ptr=(end_ptr=begin_ptr=reinterpret_cast<value_type*>(static_storage))+N;
		}
	}
	constexpr bool is_allocated_by_mmap() const
	{
		if(std::is_constant_evaluted())
			return false;
		else
			return static_cast<void*>(begin_ptr)!=static_cast<void*>(static_storage);
	}
	constexpr bool is_allocated_on_stack() const
	{
		if(std::is_constant_evaluted())
			return false;
		else
			return static_cast<void*>(begin_ptr)==static_cast<void*>(static_storage);
	}
	template<typename... Args>
	requires std::constructible_from<T,Args...>
	constexpr reference emplace_back_unchecked(Args&& ...args)
	{
		return new (std::launder(end_ptr++)) (std::forward<Args>(args)...);
	}
	constexpr void clear() noexcept
	{
		std::destroy(begin_ptr,end_ptr);
		end_ptr=begin_ptr;
	}
	inline constexpr std::size_t capacity() noexcept const
	{
		return capacity_ptr-begin_ptr;
	}
	inline constexpr std::size_t size() noexcept const
	{
		return end_ptr-begin_ptr;
	}
	inline constexpr std::size_t max_size() noexcept const
	{
		constexpr std::size_t bts{std::numeric_limits<std::size_t>::max()/sizeof(value_type)};
		return bts;
	}
private:
	constexpr void destroy_container() noexcept
	{
		clear();
		if(is_allocated_by_mmap())[[unlikely]]
			mmap_deallocate<page_bytes_hint>(begin_ptr,capacity());
	}
	constexpr void reallocate(std::size_t newcap)
	{
		mmap_allocation_unique_ptr<page_bytes_hint> uptr(newcap);
		uptr->get();
	}
public:
	template<typename... Args>
	constexpr reference emplace_back(Args&& ...args)
	{
		if(end_ptr==capacity_ptr)[[unlikely]]
			newcap();
		return new (std::launder(end_ptr++)) (std::forward<Args>(args)...);
	}
	constexpr reference pop_back() noexcept
	{
		(--end_ptr)->~value_type();
	}
/*	constexpr mmap_stack_vector(std::size_t n,T const& init_val{}) requires(std::default_initializable<T>)
	{
		if(n<=N)[[likely]]
		{
			for(std::size_t i{};i!=n;++i)
				end_ptr=new (static_data()+i*sizeof(value_type)) value_type(init_val);
			begin_ptr=end_ptr-n;
			storage=
		}
		else
		{
			mmap_allocation_unique_ptr<T,page_table_hint> uptr(n);
			for(std::size_t i{};i!=n;++i)
				new (uptr.get()+i*sizeof(value_type)) value_type(init_val);
		}
	}*/
	mmap_stack_vector(mmap_stack_vector const&) = delete;
	mmap_stack_vector& operator=(mmap_stack_vector const&) = delete;
	constexpr mmap_stack_vector(mmap_stack_vector&& bmv) noexcept
	{
		if(is_allocated_by_mmap())[[unlikely]]
		{
			begin_ptr=bmv.begin_ptr;
			end_ptr=bmv.end_ptr;
			capacity_ptr=bmv.capacity_ptr;
			bmv.capacity_ptr=bmv.end_ptr=bmv.begin_ptr={};
		}
		else
			end_ptr=std::uninitialized_move_n(bmv.begin_ptr,bmv.end_ptr-bmv.begin_ptr,begin_ptr).second;
	}
	constexpr mmap_stack_vector& operator=(mmap_stack_vector&& bmv) noexcept
	{
		if(this!=std::addressof(bmv))[[likely]]
		{
			destroy_container();
			if(is_allocated_by_mmap())[[unlikely]]
				end_ptr=std::uninitialized_move_n(bmv.begin_ptr,bmv.end_ptr-bmv.begin_ptr,begin_ptr).second;
			else
			{
				begin_ptr=bmv.begin_ptr;
				end_ptr=bmv.end_ptr;
				capacity_ptr=bmv.capacity_ptr;
				bmv.capacity_ptr=bmv.end_ptr=bmv.begin_ptr={};
			}
		}
		return *this;
	}
	constexpr pointer data() noexcept
	{
		return begin_ptr;
	}
	constexpr const_pointer data() noexcept const
	{
		return begin_ptr;
	}

	constexpr iterator begin() noexcept
	{
		return begin_ptr;
	}
	constexpr iterator end() noexcept
	{
		return end_ptr;
	}
	constexpr const_iterator begin() noexcept const
	{
		return begin_ptr;
	}
	constexpr const_iterator end() noexcept const
	{
		return end_ptr;
	}
	constexpr const_iterator cbegin() noexcept const
	{
		return begin_ptr;
	}
	constexpr const_iterator cend() noexcept const
	{
		return end_ptr;
	}

	constexpr reverse_iterator rbegin() noexcept
	{
		return {end_ptr};
	}
	constexpr reverse_iterator rend() noexcept
	{
		return {begin_ptr};
	}
	constexpr const_reverse_iterator rbegin() noexcept const
	{
		return {end_ptr};
	}
	constexpr const_reverse_iterator rend() noexcept const
	{
		return {begin_ptr};
	}
	constexpr const_reverse_iterator crbegin() noexcept const
	{
		return {end_ptr};
	}
	constexpr const_reverse_iterator crend() noexcept const
	{
		return {begin_ptr};
	}

	template<std::integral inttype>
	constexpr const_reference operator[](inttype i) noexcept const
	{
		return begin_ptr[i];
	}
	template<std::integral inttype>
	constexpr reference operator[](inttype i) noexcept
	{
		return begin_ptr[i];
	}
	constexpr ~mmap_stack_vector()
	{
		destroy_container();
	}
};

}