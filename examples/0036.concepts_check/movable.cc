#include"../../include/fast_io_device.h"


int main()
{
	static_assert(std::movable<fast_io::system_io_handle>,u8"system_io_handle not movable");
	static_assert(std::movable<fast_io::obuf>,u8"obuf_mutex not movable");
	static_assert(std::movable<fast_io::obuf_mutex>,u8"obuf_mutex not movable");
	static_assert(std::movable<fast_io::system_file>,u8"system_file not movable");
	static_assert(std::movable<fast_io::isystem_file>,u8"isystem_file not movable");
	static_assert(std::movable<fast_io::ibuf>,u8"ibuf not movable");
	static_assert(std::movable<fast_io::ibuf_mutex>,u8"ibuf_mutex not movable");
}