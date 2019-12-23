#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"

int main()
{
	fast_io::ebcdic<fast_io::obuf> eh(u8"abc.txt");
//EBCDIC to ANSI
//https://www.ibm.com/support/knowledgecenter/en/SSZJPZ_11.3.0/com.ibm.swg.im.iis.ds.parjob.adref.doc/topics/r_deeadvrf_EBCDIC_to_ASCII.html

	put(eh,0x81);//ebcdic:0x81 ansi:0x61 u8'a'
	put(eh,0xF9);//ebcdic:0xF9 ansi:0x39 u8'9'
}