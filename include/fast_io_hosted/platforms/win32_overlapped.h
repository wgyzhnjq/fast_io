#pragma once

namespace fast_io
{

namespace win32
{


enum class win32_overlapped_operations:std::uint8_t
{
read,
write,
transmit,
cancel
};
class win32_overlapped_base
{
public:
	overlapped over{};
	win32_overlapped_operations operations{};
	void (*function_ptr)(void*,std::uintptr_t,std::size_t){};
};
template<typename Func>
class win32_overlapped
{
public:
	win32_overlapped_base base{};
	Func function{};
};

inline void scheduling(void* iocp)
{
	for(;;)
	{
		std::uint32_t transferred{};
		std::uintptr_t completionkey{};
		overlapped *over{};
		if(!GetQueuedCompletionStatus(iocp,std::addressof(transferred),std::addressof(completionkey),
			std::addressof(over),std::numeric_limits<std::uint32_t>::max()))
		{
#ifdef __cpp_exceptions
			throw win32_error();
#else
			fast_terminate();
#endif
		}

		win32_overlapped_base& base(*reinterpret_cast<win32_overlapped_base*>(over));
		base.function_ptr(over,completionkey,transferred);
/*		switch(base.operations)
		{
		case win32_overlapped_operations::read:
		break;
		case win32_overlapped_operations::write:
			base.function_ptr(over,completionkey,transferred);
		break;
		case win32_overlapped_operations::transmit:
		break;
		case win32_overlapped_operations::cancel:
		break;
		}*/
	}
}

}

}