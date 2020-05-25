fast_io is a new header-only (module only in the future) experimental library to replace iostream and cstdio based on C++ 2a feature concepts.

## Compiler Support
- VS 16.3
- Clang 10.0
- GCC 10

## Platform Support
- Windows
- Linux
- MacOS
- Android

## Documentation
./doxygen/html/index.html

Since C++ 20 has not been released. No standard supporting libraries for concepts, which means a lot of Concepts Definitions are ugly. It will be changed after C++ 20 is officially published.

## Design Goal
  0. Custom Devices
  1. Exception Safe & Exception neutral
  2. As fast as possible. As close to system call as possible.
  3. Zero Overhead.
  4. As safe as possible.
  5. Binary serialization for trivially copyable types and C++ standard library containers. Serialization methods like JSON are slow and tedious for machines. read/write
  6. std::mutex mutex stream
  7. Unicode/UTF-8 support
  8. Compatible with C stdio and C++ iostream
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

## Post C++20 Plan
  1. Module support
  2. Coroutine support for async IO
  3. Improve and refactor code once [Zero-overhead deterministic exceptions](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p0709r0.pdf) are added to the standard
  4. Network handle support with coroutine. (NOT ASIO library)
  5. Partial freestanding mode

## Possible Improvements
  1. Compression/Decompression
  2. Interfaces for cloud computing algorithms like MapReduce

I'm aiming for this to be in the C++ standard library in the future :)

## How to use this library? 
Please see examples in the examples folder.

compile option:
    
`g++ -o example example.cc -O2 -std=c++2a -fconcepts`

## Benchmarks

output 10000000 size_t to file

/fast_io/tests# ./i

| Method                     | Time        |
|----------------------------|-------------|
| std::FILE*:                | 1.03459230s |
| std::ofstream:             | 0.59182820s |
| std::ofstream with tricks: | 0.39233580s |
| obuf:                      | 0.13328110s |
| obuf_mutex:                | 0.13685030s |

input 10000000 size_t from file

fast_io/tests ./j

| Method         | Time        |
|----------------|-------------|
| std::FILE*:    | 1.04546250s |
| std::ifstream: | 0.47838780s |
| ibuf:          | 0.08077780s |

### Windows:
#### Output:

output_10M_size_t.cc -O2 -std=c++2a -fconcepts

| Method                     | Time        |
|----------------------------|-------------|
| std::FILE*:                | 2.26901100s |
| std::ofstream:             | 1.03628600s |
| std::ofstream with tricks: | 0.84219500s |
| obuf:                      | 0.13401100s |
| dynamic obuf:              | 0.13586300s |
| iobuf_dynamic native_file: | 0.13000100s |
| obuf_mutex:                | 0.15303500s |

#### Input:

input_10M_size_t.cc -O2 -std=c++2a -fconcepts

| Method                              | Time        |
|-------------------------------------|-------------|
| std::FILE*:                         | 5.53888200s |
| std::ifstream:                      | 1.27124600s |
| ibuf:                               | 0.07400200s |
| dynamic standard input stream ibuf: | 0.08899900s |
| ibuf_dynamic inative_file:          | 0.07600900s |


#### Updated benchmark with trunk gcc

I did some optimizations to my fast_io library. Now there is NO reason to use charconv any more. It is insecure and the APIs are terrible. I hope isocpp would deprecate charconv in the future.

fast_io/examples/

| Method                                 | Time        |
|----------------------------------------|-------------|
| std::FILE*:                            | 0.56558740s |
| std::ofstream:                         | 0.57254780s |
| std::ofstream with tricks:             | 0.37952570s |
| std::to_chars + ofstream rdbuf tricks: | 0.16530360s |
| std::to_chars + obuf:                  | 0.12705310s |
| obuf:                                  | 0.07508470s |
| obuf text:                             | 0.13640670s |
| steam_view for ofstream:               | 0.35196200s |
| steambuf_view for ofstream:            | 0.15705550s |
| obuf ucs_view:                         | 0.15152370s |
| obuf_mutex:                            | 0.08375820s |
| fsync:                                 | 0.17738210s |
| speck128/128:                          | 0.26626790s |
