# fast_io

fast_io is a new C++20 library for extremely fast input/output and aims to replace iostream and cstdio. It is header-only (module only in the future) for easy inclusion in your project. It requires a capable C++20 compiler supporting concepts.

## Compiler Support
- GCC 11.
You can download the latest GCC compiler for windows here. https://bitbucket.org/ejsvifq_mabmip/mingw-gcc/src/master/ 
For Linux, you can watch this video to help you install the latest GCC easily. https://www.youtube.com/watch?v=qFToZjo2x24
- VS 19.26
- No Clang since Clang has not yet correctly support Concepts

## Platform Support
- Windows
- Linux

## Design Goal

Replace stdio.h and iostream

### Fast.

- As close to system call as possible.
- Locale support optional  
- Zero copy IO

### Safe

- No easily misused stuff like std::endl  
- No internal iomanip states (since it creates security issues)  
- Providing RAII for FILE\*&POSIX fd&HANDLE32

### Easy to use

- Unicode/UTF-8 support  
- Compatible with C stdio and C++ iostream  
- Binary serialization for trivially copyable types and C++ standard library containers  
- All fast_io devices can be natively put in C++ containers. std::vector<fast_io::obuf> is valid  
- Basic/Lua/Python/etc format (print, scan). No support to C and C++ since they are security hazards.
- Static I/O manipulator

### Customizability

- Exception Safe & Exception neutral  
- Native Handle Interface  
- Extremely easy to support custom devices
- std::mutex mutex stream
- Compilation time open mode parse. Supports C style open mode and C++ style open mode.
- No traits_type and EOF
- Dynamic Type Support
- Multi Process
- Memory map
- Cryptography (Under construction) to replace openssl
- debugging IO
- GUI debugging IO
- Freestanding mode
- Round-trip floating point algorithm
- Network

## Post C++20 Plan
  1. Module support
  2. Coroutine support for async IO
  3. Improve and refactor code once [Zero-overhead deterministic exceptions](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p0709r0.pdf) are added to the standard

## Possible Improvements
  1. Compression/Decompression
  2. Interfaces for cloud computing algorithms like MapReduce

I'm aiming for this to be in the C++ standard library in the future :)

## Getting Started 
Please see examples in the examples folder.

compile option:
    
`g++ -o example example.cc -Ofast -std=c++20 -s`

## Documentation

See Wiki Page: https://github.com/expnkx/fast_io/wiki

## Benchmarks

