#pragma once

namespace fast_io
{

template<std::integral CharT,typename Traits = std::char_traits<CharT>,typename Allocator = std::allocator<CharT>>
class obasic_string_ref
{
public:
	using char_type = CharT;
	using allocator_type = Allocator;
	using string_type = std::basic_string<CharT,Traits,Allocator>;
	string_type& reference;
};

template<std::integral char_type,typename traits_type,typename allocator_type>
[[nodiscard]] constexpr auto obuffer_begin(obasic_string_ref<char_type,traits_type,allocator_type> ob)
{
	return std::to_address(ob.reference.data());
}

template<std::integral char_type,typename traits_type,typename allocator_type>
[[nodiscard]] constexpr auto obuffer_curr(obasic_string_ref<char_type,traits_type,allocator_type> ob)
{
	return std::to_address(ob.reference.data()+ob.reference.size());
}

template<std::integral char_type,typename traits_type,typename allocator_type>
[[nodiscard]] constexpr auto obuffer_end(obasic_string_ref<char_type,traits_type,allocator_type> ob)
{
	return std::to_address(ob.reference.data()+ob.reference.capacity());
}

template<std::integral char_type,typename traits_type,typename allocator_type>
inline constexpr void obuffer_set_curr(obasic_string_ref<char_type,traits_type,allocator_type> ob,ch_type* ptr)
{
	details::string_hack::set_end_ptr(ob,ptr);
}

template<std::integral char_type,typename traits_type,typename allocator_type>
inline constexpr void otakeover(obasic_string_ref<char_type,traits_type,allocator_type> ob,char_type* beg_ptr,char_type* end_ptr,char_type* cap_ptr)
{
	details::string_hack::set_begin_ptr(ob,beg_ptr);
	details::string_hack::set_end_ptr(ob,end_ptr);
	details::string_hack::set_cap_ptr(ob,cap_ptr);
}

template<std::integral char_type,typename traits_type,typename allocator_type>
inline constexpr bool ocan_takeover(obasic_string_ref<char_type,traits_type,allocator_type> ob)
{
	return is_local_and_null(ob);
}


}