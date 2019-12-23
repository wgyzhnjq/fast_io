#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"
#include"../../include/fast_io_legacy.h"

int main()
{
//assume c_style_file is encoded with ebcdic even it is not.
	fast_io::ebcdic<fast_io::c_style_file> eh(u8"ebcdic_coded_file.txt",u8"wb");
//ANSI to EBCDIC
//https://www.ibm.com/support/knowledgecenter/en/SSZJPZ_11.3.0/com.ibm.swg.im.iis.ds.parjob.adref.doc/topics/r_deeadvrf_ASCII_to_EBCDIC.html

	put(eh,u8'a');//ebcdic:0x81 ansi:0x61 u8'a'
	put(eh,u8'9');//ebcdic:0xF9 ansi:0x39 u8'9'
}