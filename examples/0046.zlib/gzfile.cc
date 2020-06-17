#include<zlib.h>
#include"../../include/fast_io.h"
#include"../../include/fast_io_driver/zlib_driver.h"

int main()
{
	fast_io::zlib::gz_file gf("gzf.txt","wb");
}