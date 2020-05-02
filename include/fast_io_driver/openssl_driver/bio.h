#pragma once

namespace fast_io::openssl
{

template<stream stm>
struct fast_io_bio_method_t
{
	BIO_METHOD method{};
	explicit fast_io_bio_method_t()
	{
		if constexpr(input_stream<stm>)
		{
			method.bread=[](BIO* bbio,char* buf,std::size_t size,std::size_t* ptr,std::size_t* readd) noexcept->int
			{
				try
				{
					*readd=read(*bit_cast<stm*>(bbio->ptr),buf,buf+size);
					return 0;
				}
				catch(...)
				{
					return -1;
				}
			};
		}
		if constexpr(output_stream<stm>)
		{
			method.bwrite=[](BIO* bbio,char const* buf,std::size_t size,std::size_t* written) noexcept->int
			{
				try
				{
					*written=write(*bit_cast<stm*>(bbio->ptr),buf,buf+size);
					return 0;
				}
				catch(...)
				{
					return -1;
				}
			};
		}
		method.destroy=[](BIO* bbio) noexcept -> int
		{
			delete bit_cast<stm*>(bbio.ptr);
			return 0;
		}
		auto info(typeid(stream));
		method.name=typeid(stream).name();
		constexpr int value(BIO_TYPE_DESCRIPTOR-BIO_TYPE_START);
		static_assert(0<value);
		method.type=static_cast<int>(info.hash()%value+BIO_TYPE_START);
	}
};

template<stream stm>
fast_io_bio_method_t<stm> fast_io_bio_method{};

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
	template<stream stm,Args ...args>
	requires std::constructible_from<stm,Args...>
	basic_bio_file(c_file_cookie_t,std::in_place_type_t<stm>,Args&& ...args):basic_bio_io_observer(BIO_new(std::addressof(fast_io_bio_method<stm>)))
	{
		if(bio==nullptr)[[unlikely]]
			throw openssl_error("BIO_new() failed");
		basic_bio_file<ch_type> self(this->native_handle());
		self.native_handle()->ptr=bit_cast<void*>(new stm(std::forward<Args>(args)...));
		self.release();
	}
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