
#include "../../../include/fast_io_device.h"
#include <cstdarg>
/* malloc wrapper function */

fast_io::onative_file nf("wrap_log.txt");

extern "C" int __real_vprintf(char const* format, va_list ap) noexcept;

extern "C" int __wrap_vprintf(char const* format, va_list ap) noexcept
{
	debug_println(nf,std::chrono::system_clock::now()," ",__FILE__ ,":__wrap_vprintf ",format);
	return __real_vprintf(format,ap);
}

extern "C" int __real_printf(char const* format, ... ) noexcept;

extern "C" int __wrap_printf(char const* format, ... ) noexcept
{
	debug_println(nf,std::chrono::system_clock::now()," ",__FILE__ ,":__wrap_printf fp: format: ",format);
	std::va_list args;
	va_start(args, format);
	int ret = __real_vprintf(format, args);
	va_end(args);
	return ret;
}


extern "C" int __real_vfprintf(FILE *s, char const* format, va_list ap) noexcept;

extern "C" int __wrap_vfprintf(FILE *s, char const* format, va_list ap) noexcept
{
	debug_println(nf,std::chrono::system_clock::now()," ",__FILE__ ,":__wrap_vfprintf(",fast_io::unsigned_view(s),",",format,")");
	return __real_vfprintf(s,format,ap);
}

extern "C" int __real_fprintf(std::FILE* stream, char const* format, ... ) noexcept;

extern "C" int __wrap_fprintf(std::FILE* stream, char const* format, ... ) noexcept
{
	debug_println(nf,std::chrono::system_clock::now()," ",__FILE__ ,":__wrap_fprintf fp:",fast_io::unsigned_view(stream),",format: ",format);
	std::va_list args;
	va_start(args, format);
	int ret = __real_vfprintf(stream, format, args);
	va_end(args);
	return ret;
}


extern "C" int __real_vsprintf( char* buffer,  char const* format, va_list vlist ) noexcept;

extern "C" int __wrap_vsprintf( char* buffer, char const* format, va_list vlist ) noexcept
{
	debug_println(nf,std::chrono::system_clock::now()," ",__FILE__ ,":__wrap_vsprintf(",fast_io::unsigned_view(buffer),",",format,")");
	return __real_vsprintf(buffer,format,vlist);
}

extern "C" int __real_sprintf(char* buffer, char const* format, ... ) noexcept;

extern "C" int __wrap_sprintf(char* buffer, char const* format, ... ) noexcept
{
	debug_println(nf,std::chrono::system_clock::now()," ",__FILE__ ,":__wrap_sprintf fp:",fast_io::unsigned_view(buffer),",format: ",format);
	std::va_list args;
	va_start(args, format);
	int ret = __real_vsprintf(buffer, format, args);
	va_end(args);
	return ret;
}

extern "C" int __real_vsnprintf( char* buffer, std::size_t buf_size,  char const* format, va_list vlist ) noexcept;

extern "C" int __wrap_vsnprintf( char* buffer, std::size_t buf_size, char const* format, va_list vlist ) noexcept
{
	debug_println(nf,std::chrono::system_clock::now()," ",__FILE__ ,":__wrap_vsnprintf(",fast_io::unsigned_view(buffer),",",format,")");
	return __real_vsnprintf(buffer,buf_size,format,vlist);
}

extern "C" int __real_snprintf(char* buffer, std::size_t buf_size, char const* format, ... ) noexcept;

extern "C" int __wrap_snprintf(char* buffer, std::size_t buf_size, char const* format, ... ) noexcept
{
	debug_println(nf,std::chrono::system_clock::now()," ",__FILE__ ,":__wrap_snprintf fp:",fast_io::unsigned_view(buffer),",format: ",format);
	std::va_list args;
	va_start(args, format);
	int ret = __real_vsnprintf(buffer, buf_size, format, args);
	va_end(args);
	return ret;
}

/*

g++ -o example example.cc -Ofast -std=c++20 -s -Wl,--wrap,vfprintf -Wl,--wrap,fprintf -Wl,--wrap,vsprintf -Wl,--wrap,sprintf -Wl,--wrap,vsnprintf fprintf.cc
*/