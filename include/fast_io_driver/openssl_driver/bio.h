#pragma once

namespace fast_io::openssl
{

template<std::integral ch_type>
class basic_bio_io_observer
{
public:
	using native_handle_type = BIO*;
	using char_type = ch_type;
	native_handle_type bio{};
	constexpr operator bool() const noexcept
	{
		return bio;
	}
	constexpr native_handle_type& native_handle() const noexcept
	{
		return bio;
	}
	constexpr native_handle_type& native_handle() noexcept
	{
		return bio;
	}
	constexpr native_handle_handle release() noexcept
	{
		auto temp{bio};
		bio=nullptr;
		return temp;
	}
};

template<std::integral ch_type>
class basic_bio_file:public basic_bio_io_observer<ch_type>
{
public:
	using native_handle_type = BIO*;
	using char_type = ch_type;
	constexpr basic_bio_file()=default;
	constexpr basic_bio_file(native_handle_type bio):basic_bio_io_observer<ch_type>(bio){}
	basic_bio_file(basic_bio_file const&)=delete;
	basic_bio_file& operator=(basic_bio_file const&)=delete;
	constexpr basic_bio_file& operator=(basic_bio_file&& bf) noexcept:basic_bio_io_observer<ch_type>(bf.native_handle())
	{
		bf.native_handle()=nullptr;
	}
	basic_bio_file& operator=(basic_bio_file&& bf) noexcept
	{
		if(bf.native_handle()==this->native_handle())
			return *this;
		if(this->native_handle())[[likely]]
			BIO_free(this->native_handle());
		this->native_handle()=bf.native_handle();
		bf.native_handle()=nullptr;
		return *this;
	}
	~basic_bio_file()
	{
		if(this->native_handle())[[likely]]
			BIO_free(this->native_handle());
	}
};

}