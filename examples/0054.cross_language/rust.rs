extern "C"
{
/*void cxx_fast_io_bufferred_release(void* deviceptr) CXX_FAST_IO_NOEXCEPT;
int cxx_fast_io_bufferred_acquire_file(void** generated_device,char const* filename,char const* open_mode) CXX_FAST_IO_NOEXCEPT;
int cxx_fast_io_bufferred_print_size_t(void* device,size_t value) CXX_FAST_IO_NOEXCEPT;
int cxx_fast_io_bufferred_print_ptrdiff_t(void* device,ptrdiff_t value) CXX_FAST_IO_NOEXCEPT;
int cxx_fast_io_bufferred_print_double(void* device,double value) CXX_FAST_IO_NOEXCEPT;
int cxx_fast_io_bufferred_print_c_str(void* device,char const* c_str) CXX_FAST_IO_NOEXCEPT;
int cxx_fast_io_bufferred_put(void* device,char ch) CXX_FAST_IO_NOEXCEPT;*/
fn cxx_fast_io_bufferred_release(handle:*mut std::os::raw::c_void)->std::os::raw::c_int;
//fn cxx_fast_io_bufferred_acquire_file(**libc::c_void,*const libc::c_char,*const libc::c_char)->libc::c_int;
//fn cxx_fast_io_bufferred_print_size_t(*libc::c_void,usize)->libc::c_int;
//fn cxx_fast_io_bufferred_print_double(*libc::c_void,f64)->libc::c_int;

}
#[derive(Default)]
struct CxxFastIoBufferredHandle
{
handle:*mut std::os::raw::c_void
}

impl Drop for CxxFastIoBufferredHandle
{
fn drop(&mut self)
{
unsafe
{
	cxx_fast_io_bufferred_release(self.handle);
}
}
}

fn main()
{
//	let handle=CxxFastIoBufferredHandle{handle:std::ptr::null};
}