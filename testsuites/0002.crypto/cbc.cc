#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"
#include"../../include/fast_io_crypto.h"

int main()
{
    std::array<unsigned char, 16> key{1,2,3,4,5,6,7,8,1,2,3,4,5,6,7,8};
    std::array<unsigned char, 16> iv{1,2,3,4,5,6,7,8,1,2,3,4,5,6,7,8};
    {
        fast_io::crypto::ocbc_decrypt<fast_io::obuf_file, fast_io::crypto::speck::speck_dec_128_128> 
            ob(std::piecewise_construct,std::forward_as_tuple("text.txt"),
            std::forward_as_tuple(std::as_writable_bytes(std::span(key)), std::as_writable_bytes(std::span(iv))));
        //print("xxx");
        print(ob,"hello world\nxxxx\n8988\n");
    }
    {
        fast_io::crypto::icbc_encrypt<fast_io::ibuf_file, fast_io::crypto::speck::speck_enc_128_128> 
            ib(std::piecewise_construct,std::forward_as_tuple("text.txt"),
            std::forward_as_tuple(std::as_writable_bytes(std::span(key)), std::as_writable_bytes(std::span(iv))));
        //print("xxx");
        //std::array<char, 20> arr;
        //read(ib, arr);
        //write(fast_io::out, arr);
        //fast_io::obuf_file ob("out.txt");
        transmit(fast_io::out,ib);
    }
    //ob.iv = {1,2,3,4,5,6,7,8,1,2,3,4,5,6,7,8};
    //fast_io::crypto::cbc_decrypt<fast_io::crypto::speck::speck_dec_128_128> enc{std::as_writable_bytes(std::span(key)), std::as_writable_bytes(std::span(iv))};
    //ob.key = {1,2,3,4,5,6,7,8,1,2,3,4,5,6,7,8};
    //write()
	//fast_io::oebcdic_to_ascii<fast_io::obuf_file> obf("ote.txt");
	//transmit(obf,ibf);
}