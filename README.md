# Othello (C++23)
Commonly 2D arrays are used to represent the state, but this implementation uses Bitboards for minimal memory usage and fast operations on 64 bit systems.
Bitboards are unsigned 64-bit integers where each bit represent a square on the board. An Othello board is 8x8, so 64 bits are sufficient to represent the entire board state.
Using bitwise operations, the game state can be manipulated efficiently.

# Course
This project was started to learn C++ within the context of a certified C++ advanced course.

The Course plan includes:
- C++ basics (Objects, Streams, namespaces etc.)
- STL library (container, templates, algorithms)
- Working with Databases and files
- Unit Testing
- Networking
- Multithreading
- GUI programming with Qt

All topics can be covered within the context of a game project, which makes it more engaging and practical. Therefore, all topics are implemented in the context of an Othello game.

Outside of the course plan, the following topics are also covered:
- C++23 standard compliance
- Documentation with Doxygen
- Strict Static Analysis with Clang-Tidy
- Design patterns (MVC, Repository Pattern, Dependency Injection)
- Cmake build system
- Hardware acceleration with SIMD instructions on modern CPUs (AVX2, AVX-512)

# SIMD (AVX2, AVX512)
Bitboards are already highly efficient however, through the use of SIMD instructions, the performance can be further improved.
For example, the move generation can be accelerated by processing multiple directions in parallel through the use of AVX2 or AVX-512 instructions (on Intel and AMD). 
Without SIMD, the game processes each direction sequentially, which is still fast enough for most applications. When using AVX2 we can process 4 direction (256-bit) in parallel, and with AVX-512 (512 bit) we can process all 8 directions in parallel.
An example implementation of the move generation using AVX-512 is included. However, this was not tested due to my CPU not supporting AVX-512. The AVX2 implementation is tested and works correctly.






