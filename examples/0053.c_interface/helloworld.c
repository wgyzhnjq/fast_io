#include"../../include/fast_io_c_interface.h"

int main()
{
	void* out_handle=NULL;
	cxx_fast_io_bufferred_acquire_file(&out_handle,u8"hello.txt",u8"wb");
	cxx_fast_io_bufferred_print_c_str(out_handle,u8"Hello World\n");
	cxx_fast_io_bufferred_release(out_handle);
}
