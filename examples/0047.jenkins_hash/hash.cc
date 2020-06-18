#include"../../include/fast_io.h"
#include"../../include/fast_io_crypto/hash/jenkins.h"
//https://github.com/vkandy/jenkins-hash-java/blob/master/test/JenkinsHashTest.java
int main()
{
	static_assert(fast_io::jenkins_hash64("")==0xdeadbeefdeadbeefL);
	static_assert(fast_io::jenkins_hash("",0,0xdeadbeef)==0xbd5b7ddedeadbeefL);
	static_assert(fast_io::jenkins_hash("",0xdeadbeef,0xdeadbeef)==0x9c093ccdbd5b7ddeL);
	static_assert(fast_io::jenkins_hash64("Four score and seven years ago")==0x17770551ce7226e6L);
	static_assert(fast_io::jenkins_hash("Four score and seven years ago",0,1)==0xe3607caebd371de4L);
	static_assert(fast_io::jenkins_hash("Four score and seven years ago",1,0)==0xcd6281616cbea4b3L);
	static_assert(fast_io::jenkins_hash("Four score and seven years ago",0)==0x17770551);
	static_assert(fast_io::jenkins_hash("Four score and seven years ago",1)==0xcd628161);
}