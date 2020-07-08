#pragma once

namespace fast_io::zlib
{

namespace details
{

struct gz_state_model{
        /* exposed contents for gzgetc() macro */
    struct gzFile_s x;      /* "x" for exposed */
                            /* x.have: number of bytes available at x.next */
                            /* x.next: next output data to deliver or write */
                            /* x.pos: current position in uncompressed data */
        /* used for both reading and writing */
    int mode;               /* see gzip modes above */
    int fd;                 /* file descriptor */
    char *path;             /* path or fd for error messages */
    unsigned size;          /* buffer size, zero if not allocated yet */
    unsigned want;          /* requested buffer size, default is GZBUFSIZE */
    unsigned char *in;      /* input buffer */
    unsigned char *out;     /* output buffer (double-sized when reading) */
    int direct;             /* 0 if processing gzip, 1 if transparent */
        /* just for reading */
    int how;                /* 0: get header, 1: copy, 2: decompress */
    z_off64_t start;        /* where the gzip data started, for rewinding */
    int eof;                /* true if end of input file reached */
    int past;               /* true if read requested past end */
        /* just for writing */
    int level;              /* compression level */
    int strategy;           /* compression strategy */
        /* seek request */
    z_off64_t skip;         /* amount to skip (already rewound if backwards) */
    int seek;               /* true if seek request pending */
        /* error information */
    int err;                /* error code */
    char *msg;              /* error message */
        /* zlib inflate or deflate stream */
    z_stream strm;          /* stream structure in-place (not a pointer) */
};

inline int hack_gz_file_fd(gzFile gzfile)
{
	int fdn;
	memcpy(std::addressof(fdn),reinterpret_cast<std::byte const*>(gzfile)+offsetof(gz_state_model,fd),sizeof(int));
	return fdn;
}
/*
inline char* hack_gz_file_in(gzFile gzfile)
{
	char* ptr;
	memcpy(std::addressof(ptr),reinterpret_cast<std::byte const*>(gzfile)+offsetof(gz_state_model,in),sizeof(char *));
	return ptr;
}
inline char* hack_gz_file_out(gzFile gzfile)
{
	char* ptr;
	memcpy(std::addressof(ptr),reinterpret_cast<std::byte const*>(gzfile)+offsetof(gz_state_model,out),sizeof(char *));
	return ptr;
}
*/
}

template<std::integral ch_type>
class basic_gz_io_observer
{
public:
	using native_handle_type = gzFile;
	using char_type = ch_type;
	native_handle_type gzfile{};
	constexpr auto& native_handle() noexcept
	{
		return gzfile;
	}
	constexpr auto& native_handle() const noexcept
	{
		return gzfile;
	}
	explicit operator bool() const noexcept
	{
		return gzfile;
	}
	explicit operator basic_posix_io_observer<char_type>() const noexcept
	{
		return {details::hack_gz_file_fd(gzfile)};
	}
#if defined(__WINNT__) || defined(_MSC_VER)
	explicit operator basic_win32_io_observer<char_type>() const
	{
		return static_cast<basic_win32_io_observer<char_type>>(static_cast<basic_posix_io_observer<char_type>>(*this));
	}
#endif
};

template<std::integral ch_type>
class basic_gz_file:public basic_gz_io_observer<ch_type>
{
public:
	using native_handle_type = gzFile;
	using char_type = ch_type;
	constexpr basic_gz_file()=default;
	constexpr basic_gz_file(native_handle_type hd) noexcept:basic_gz_io_observer<char_type>(hd){}
	basic_gz_file(basic_gz_file const&)=delete;
	basic_gz_file& operator=(basic_gz_file const&)=delete;
	constexpr basic_gz_file(basic_gz_file&& bmv) noexcept:basic_gz_io_observer<char_type>(bmv.native_handle())
	{
		bmv.native_handle()=nullptr;
	}
	basic_gz_file& operator=(basic_gz_file&& bmv) noexcept
	{
		if(std::addressof(bmv)==this)[[unlikely]]
			return *this;
		if(this->native_handle())[[likely]]
			gzclose(this->native_handle());
		this->native_handle()=bmv.native_handle();
		bmv.native_handle()=nullptr;
		return *this;
	}
	~basic_gz_file()
	{
		if(this->native_handle())[[likely]]
			gzclose(this->native_handle());
	}


	basic_gz_file(basic_posix_io_handle<char_type>&& posix_handle,std::string_view mode):
		basic_gz_io_observer<char_type>(gzdopen(posix_handle.native_handle(),mode.data()))
	{
		if(this->native_handle()==nullptr)
			FIO_POSIX_ERROR();
		posix_handle.detach();
	}

	basic_gz_file(basic_posix_io_handle<char_type>&& posix_handle,open_mode om):
		basic_gz_file(std::move(posix_handle),to_c_mode(om)){}
	template<open_mode om>
	basic_gz_file(basic_posix_io_handle<char_type>&& posix_handle,open_interface_t<om>):
		basic_gz_file(std::move(posix_handle),fast_io::details::c_open_mode<om>::value){}

#if defined(__WINNT__) || defined(_MSC_VER)
//windows specific. open posix file from win32 io handle
	basic_gz_file(basic_win32_io_handle<char_type>&& win32_handle,std::string_view mode):
		basic_gz_file(basic_posix_file<char_type>(std::move(win32_handle),mode),mode)
	{
	}
	basic_gz_file(basic_win32_io_handle<char_type>&& win32_handle,open_mode om):
		basic_gz_file(basic_posix_file<char_type>(std::move(win32_handle),om),to_c_mode(om))
	{
	}
	template<open_mode om>
	basic_gz_file(basic_win32_io_handle<char_type>&& win32_handle,open_interface_t<om>):
		basic_gz_file(basic_posix_file<char_type>(std::move(win32_handle),open_interface<om>),
			fast_io::details::c_open_mode<om>::value)//open::c_style_interface_t<om>::mode)
	{
	}
#endif


	template<open_mode om,typename... Args>
	basic_gz_file(std::string_view file,open_interface_t<om>,Args&& ...args):
		basic_gz_file(basic_posix_file<char_type>(file,open_interface<om>,std::forward<Args>(args)...),
			open_interface<om>)
	{}
	template<typename... Args>
	basic_gz_file(std::string_view file,open_mode om,Args&& ...args):
		basic_gz_file(basic_posix_file<char_type>(file,om,std::forward<Args>(args)...),om)
	{}
	template<typename... Args>
	basic_gz_file(std::string_view file,std::string_view mode,Args&& ...args):
		basic_gz_file(basic_posix_file<char_type>(file,mode,std::forward<Args>(args)...),mode)
	{}

};

using gz_io_observer = basic_gz_io_observer<char>;
using gz_file = basic_gz_file<char>;


template<std::integral char_type,std::contiguous_iterator Iter>
requires (std::same_as<char_type,std::iter_value_t<Iter>>||std::same_as<char,char_type>)
inline Iter read(basic_gz_io_observer<char_type> giob,Iter b,Iter e)
{
	if constexpr(std::same_as<char_type,std::iter_value_t<Iter>>)
	{
		std::size_t to_read((e-b)*sizeof(*b));
		if constexpr(sizeof(unsigned)<sizeof(std::size_t))
			if(static_cast<std::size_t>(std::numeric_limits<unsigned>::max())<to_read)
				to_read=std::numeric_limits<unsigned>::max();
		int readed{gzread(giob.gzfile,std::to_address(b),static_cast<unsigned>(to_read))};
		if(readed==-1)
			FIO_POSIX_ERROR();
		return b+static_cast<std::size_t>(readed)/sizeof(*b);
	}
	else
		return b+(read(giob,reinterpret_cast<char*>(std::to_address(b)),reinterpret_cast<char*>(std::to_address(e)))-reinterpret_cast<char*>(std::to_address(b)))/sizeof(*b);
}

template<std::integral char_type,std::contiguous_iterator Iter>
requires (std::same_as<char_type,std::iter_value_t<Iter>>||std::same_as<char,char_type>)
inline Iter write(basic_gz_io_observer<char_type> giob,Iter b,Iter e)
{
	if constexpr(std::same_as<char_type,std::iter_value_t<Iter>>)
	{
		std::size_t to_write((e-b)*sizeof(*b));
		if constexpr(sizeof(unsigned)<sizeof(std::size_t))
			if(static_cast<std::size_t>(std::numeric_limits<unsigned>::max())<to_write)
				to_write=std::numeric_limits<unsigned>::max();
		int written{gzwrite(giob.gzfile,std::to_address(b),static_cast<unsigned>(to_write))};
		if(written<0)
			FIO_POSIX_ERROR();
		return b+static_cast<std::size_t>(written)/sizeof(*b);
	}
	else
		return b+(write(giob,reinterpret_cast<char const*>(std::to_address(b)),reinterpret_cast<char const*>(std::to_address(e)))-reinterpret_cast<char const*>(std::to_address(b)))/sizeof(*b);
}

template<std::integral char_type>
inline void flush(basic_gz_io_observer<char_type> giob)
{
	if(gzflush(giob.gzfile))
		FIO_POSIX_ERROR();
}

static_assert(input_stream<gz_file>);
static_assert(output_stream<gz_file>);


template<std::integral char_type>
using basic_ibuf_gz_file=basic_ibuf<basic_gz_file<char_type>>;

using ibuf_gz_file = basic_ibuf_gz_file<char>;

}