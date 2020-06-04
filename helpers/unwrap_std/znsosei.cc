#include<ostream>
#include"../../include/fast_io.h"
#include"../../include/fast_io_legacy_impl/cpp/streambuf_io_observer.h"

namespace fast_io
{
template<typename T>
inline std::ostream& wrapped_function(std::ostream& out,T v)
{
	fast_io::streambuf_io_observer iob{out.rdbuf()};
	print(iob,v);
	return out;
}
}

extern "C" std::ostream& __wrap__ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_x(std::ostream& out,char ch)
{
	fast_io::streambuf_io_observer iob{out.rdbuf()};
	put(iob,ch);
	return out;
}

extern "C" std::ostream& __wrap__ZNSolsEs(std::ostream& out,short v)
{
	return fast_io::wrapped_function(out,v);
}

extern "C" std::ostream& __wrap__ZNSolsEi(std::ostream& out,int v)
{
	return fast_io::wrapped_function(out,v);
}

extern "C" std::ostream& __wrap__ZNSo9_M_insertImEERSoT_(std::ostream& out,unsigned short v)
{
	return fast_io::wrapped_function(out,v);
}

extern "C" std::ostream& __wrap__ZNSo9_M_insertIlEERSoT_(std::ostream& out,long v)
{
	return fast_io::wrapped_function(out,v);
}

extern "C" std::ostream& __wrap__ZNSo9_M_insertIxEERSoT_(std::ostream& out,long long v)
{
	return fast_io::wrapped_function(out,v);
}

extern "C" std::ostream& __wrap__ZNSo9_M_insertIyEERSoT_(std::ostream& out,unsigned long long v)
{
	return fast_io::wrapped_function(out,v);
}

extern "C" std::ostream& __wrap__ZNSo9_M_insertIdEERSoT_(std::ostream& out,double v)
{
	return fast_io::wrapped_function(out,v);
}

extern "C" std::ostream& __wrap__ZNSo9_M_insertIeEERSoT_(std::ostream& out,long double v)
{
	return fast_io::wrapped_function(out,v);
}