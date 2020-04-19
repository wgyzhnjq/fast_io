This is a new header-only (module only in the future) Experimental library to replace iostream and cstdio based on C++ 2a feature concepts. Currently, the only compiler which supports Concepts TS is GCC. Supports VC preview as well. It can compile on clang concepts experimental. I have tested all the compilers.

It can even work on android !! 

## Documents
./doxygen/html/index.html

Since C++ 20 has not been released. No standard supporting libraries for concepts, which means a lot of Concepts Definitions are ugly. It will be changed after C++ 20 is officially published.

## Design goal.
  0. Custom Devices
  1. Exception Safe & Exception neutral
  2. As fast as possible. As close to system call as possible.
  3. Zero Overhead.
  4. As safe as possible.
  5. Binary serialization for trivially copyable types and C++ standard library containers. Serialization methods like JSON are slow and tedious for machines. read/write
  6. std::mutex mutex stream
  7. Unicode/UTF-8 support
  8. Compatible with C stdio/ C++ iostream
  9. Native Handle Interface
  10. support C style io format (fprint). Basic/Lua/Python/etc format (print, scan). NO LONGER SUPPORTS C++ style io format (<<,>>) since we have varadics templates now. Using operator overloading incurs more overhead including compilation and runtime. I could not do optimization like avoiding multiple locking.
  11. Compilation time open mode parse. Supports C style open mode and C++ style open mode.
  12. No internal iomanip states (since it creates security issues)
  13. Extremely easy to support custom devices
  14. No easily misused stuff like std::endl
  15. No integrated locale
  16. No traits_type and EOF
  17. All fast_io devices can be natively put in C++ containers. std::vector<fast_io::obuf> is valid
  18. Providing RAII for FILE*&POSIX file id
  19. Dynamic Type Support
  20. Pipe line support
  21. Zero copy IO
  22. Cryptography (Under construction) to replace openssl
  23. debugging IO
  24. GUI debugger (calling win32 apis MessageBox for those who are doing none console programming)

## Future Plan After C++ 20
  1. Module support
  2. Coroutine support for async IO
  3. if this proposal would be released. Probably remove more stuffs here
     Zero-overhead deterministic exceptions: Throwing values
     http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p0709r0.pdf
  4. Network handle support with coroutine. (NOT ASIO library)
     NETWORK SUPPORT IS BECOMING MORE AND MORE IMPORTANT IN MODERN WORLD
  5. Partial Freestanding mode

## Possible Stuff in the future
  1. Compression/Decompression
  2. Interfaces for cloud computing algorithms like MapReduce

I want this to be in the C++ standard library in the future. :)

## How to use this library? 
Please see examples in the examples folder.

compile option:
    
	g++ -o example example.cc -O2 -std=c++2a -fconcepts

Compiler recommendation under windows:

https://gcc-mcf.lhmouse.com/

## Benchmark

output 10000000 size_t to file
```
root@XXXXXX:/mnt/d/hg/fast_io/tests# ./i

std::FILE*:     1.03459230s

std::ofstream:  0.59182820s

std::ofstream with tricks:      0.39233580s

obuf:   0.13328110s

obuf_mutex:     0.13685030s
```

input 10000000 size_t from file
```
root@XXXXXX:/mnt/d/hg/fast_io/tests# ./j

std::FILE*:     1.04546250s

std::ifstream:  0.47838780s

ibuf:   0.08077780s
```

### Windows:
Output:
```
g++ -o output_10M_size_t output_10M_size_t.cc -O2 -std=c++2a -fconcepts

Process started (PID=1580) >>>

<<< Process finished (PID=1580). (Exit code 0)

output_10M_size_t

Process started (PID=1208) >>>

std::FILE*:	2.26901100s

std::ofstream:	1.03628600s

std::ofstream with tricks:	0.84219500s

obuf:	0.13401100s

dynamic obuf:	0.13586300s

iobuf_dynamic native_file:	0.13000100s

obuf_mutex:	0.15303500s
```

Input:
```
g++ -o input_10M_size_t input_10M_size_t.cc -O2 -std=c++2a -fconcepts

Process started (PID=11856) >>>

<<< Process finished (PID=11856). (Exit code 0)

input_10M_size_t

Process started (PID=21088) >>>

std::FILE*:	5.53888200s

std::ifstream:	1.27124600s

ibuf:	0.07400200s

dynamic standard input stream ibuf:	0.08899900s

ibuf_dynamic inative_file:	0.07600900s
```




visual studio preview 2019
https://visualstudio.microsoft.com/vs/preview/



Updated benchmark with trunk gcc

I did some optimizations to my fast_io library. Now there is NO reason to use charconv any more. It is insecure and the APIs are terrible. I hope isocpp would deprecate charconv in the future.

```

cqwrteur@DESKTOP-7H7UHQ9:~/fast_io/examples/build$ g++ --version

g++ (cqwrteur) 10.0.0 20191031 (experimental)

Copyright (C) 2019 Free Software Foundation, Inc.

This is free software; see the source for copying conditions.  There is NO 

warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

cqwrteur@DESKTOP-7H7UHQ9:~/fast_io/examples/build$ ./output_10M_size_t

std::FILE*:     0.56558740s

std::ofstream:  0.57254780s

std::ofstream with tricks:      0.37952570s

std::to_chars + ofstream rdbuf tricks:  0.16530360s

std::to_chars + obuf:   0.12705310s

obuf:   0.07508470s

obuf text:      0.13640670s

steam_view for ofstream:        0.35196200s

steambuf_view for ofstream:     0.15705550s

obuf ucs_view:  0.15152370s

obuf_mutex:     0.08375820s

fsync:  0.17738210s

speck128/128:   0.26626790s
```



fmt int-benchmark


cqwrteur@Home-Server:~/format-benchmark/build$ ./int-benchmark
 1  27422
 2 247633
 3 449843
 4 247585
 5  24802
 6   2431
 7    263
 8     18
 9      3
10      0
2020-04-19 03:38:10
Running ./int-benchmark
Run on (32 X 3500 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x16)
  L1 Instruction 32 KiB (x16)
  L2 Unified 512 KiB (x16)
  L3 Unified 16384 KiB (x4)
Load Average: 2.16, 1.84, 1.72
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
-------------------------------------------------------------------------------
Benchmark                     Time             CPU   Iterations UserCounters...
-------------------------------------------------------------------------------
sprintf                60485763 ns     60474022 ns           11 items_per_second=16.536M/s
ostringstream          51180157 ns     51171861 ns           14 items_per_second=19.542M/s
to_string              11014153 ns     11011117 ns           64 items_per_second=90.8173M/s
format                 37004954 ns     36998384 ns           19 items_per_second=27.0282M/s
format_to              23252236 ns     23247699 ns           30 items_per_second=43.015M/s
format_to_compile      10718580 ns     10716635 ns           65 items_per_second=93.3129M/s
format_int              7867424 ns      7866027 ns           89 items_per_second=127.129M/s
lexical_cast           28524279 ns     28517046 ns           25 items_per_second=35.0667M/s
boost_format          249655844 ns    249592177 ns            3 items_per_second=4.00654M/s
boost_karma_generate    8663729 ns      8661537 ns           81 items_per_second=115.453M/s
voigt_itostr           14439111 ns     14436228 ns           49 items_per_second=69.2702M/s
decimal_from            9162319 ns      9160592 ns           76 items_per_second=109.163M/s
stout_ltoa             18511214 ns     18507607 ns           38 items_per_second=54.0318M/s
fast_io_ospan_res       6871917 ns      6870708 ns          102 items_per_second=145.545M/s
fast_io_concat          7967591 ns      7966162 ns           88 items_per_second=125.531M/s



2020-04-19 03:39:11
Running ./double-benchmark
Run on (32 X 3500 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x16)
  L1 Instruction 32 KiB (x16)
  L2 Unified 512 KiB (x16)
  L3 Unified 16384 KiB (x4)
Load Average: 1.78, 1.81, 1.72
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
---------------------------------------------------------
Benchmark               Time             CPU   Iterations
---------------------------------------------------------
sprintf            550514 ns       550435 ns         1273
format_to          116631 ns       116611 ns         6009
dtoa_milo           75115 ns        75103 ns         9299
fast_io_ospan       19955 ns        19950 ns        33426
fast_io_concat      43292 ns        43286 ns        16414