# SIMDe-LZSSE

[LZSSE](https://github.com/ConorStokes/LZSSE/) has a hard dependency on SSE4.1, which prevents it from working on other architectures, or even x86/x86_64 machines without support for the SSE4.1 instruction set.  According to the [Steam Hardware Survey](http://store.steampowered.com/hwsurvey), SSE4.1 currently has just under 90% penetration, and of course that is only for machines with Steam installed (which is a pretty big bias).

This is a fork of [LZSSE](https://github.com/ConorStokes/LZSSE/) which uses [SIMDe](https://github.com/nemequ/simde) to allow for LZSSE (de)compression on platforms where SSE4.1 is not supported, including other architectures (such as ARM).  SIMDe is still under heavy development, and this fork is mostly just a proof-of-concept at this point, but it is fully functional.

For machines with SSE4.1 support, there should be no performance impact.  The SSE4.1 intrinsics will be called, and the compiler should be capable of optimizing away any overhead associated with SIMDe.

For machines which don't natively support the instructions used, SIMDe will emulate them using portable fallbacks.  These are obviously slower, but at least the code works.

Note that a mix of the two is quite possible; for example, a CPU may support SSE2 and SSSE3 but not SSE4.1, in which case SSE4.1 functions will be emulated but instructions supported by the target will be passed through.

The process of switching to SIMDe was quite trivial:

 1. Include the relevant header from SIMDe.  In this case, `sse4.1.h`.
 2. Rename relevant types and functions.  `__m128i` becomes `simde__m128i`, and `_mm_*` functions become `simde_mm_*`.
 3. Change compiler flags from `-msse4.1` to `-march=native`.

For better performance, it's also helpful to enable OpenMP 4.0 SIMD support; on GCC, you can do this by passing `-fopenmp-simd -DSIMDE_ENABLE_OPENMP`.

I'll try to keep this up to date with LZSSE, but I will not accept any changes directly to this repository not directly related to porting to SIMDe.  If you find a bug, please file it with LZSSE or SIMDe, whichever would be more appropriate.

## SIMDe Performance

This is based on some informal testing with g++ 6.3 against enwik8.  3 iterations of each test, middle value used.

If provided the same compiler flags (`-march=native -O3`), results for LZSSE and LZSSE-SIMDe are effectively the same (sometimes one is faster, sometimes the other).  So **SIMDe doesn't make things worse**, which is important.

Forcing SIMDe to ignore intrinsics (by passing `-DSIMDE_NO_NATIVE`) makes things more interesting.  LZSSE still uses the intrinsics, but the SIMDe version has to rely on the compiler's auto-vectorizer, so obviously there will be a slow-down.  This is a **worst-case scenario** for SIMDe; often you'll be able to use at least a subset of the ISA extension (*e.g.*, maybe the machine has SSE2 but not SSE4.1), so in reality performance will probably be somewhere between these values:

| Variant | LZSSE Compress | SIMDe Compress | LZSSE Decompress | SIMDe Decompress |
| ------- | -------------- | -------------- | ---------------- | ---------------- |
|       2 | 12.519         | 19.612         | 0.084            | 0.252            |
|       4 | 10.708         | 16.205         | 0.082            | 0.278            |
|       8 | 11.172         | 17.450         | 0.083            | 0.289            |

The same flags were used for all configurations `-DSIMDE_NO_NATIVE -DSIMDE_ENABLE_OPENMP -fopenmp-simd -march=native -O3`.

# LZSSE
[LZSS](https://en.wikipedia.org/wiki/Lempel%E2%80%93Ziv%E2%80%93Storer%E2%80%93Szymanski) designed for a branchless SSE decompression implementation.

Three variants:
- LZSSE2, for high compression files with small literal runs.
- LZSSE4, for a more balanced mix of literals and matches.
- LZSSE8, for lower compression data with longer runs of matches.

All three variants have an optimal parser implementation, which uses a quite strong match finder (very similar to LzFind) combined with a Storer-Szymanksi style parse. LZSSE4 and LZSSE8 have "fast" compressor implementations, which use a simple hash table based matching and a greedy parse.

Currently LZSSE8 is the recommended variant to use in the general case, as it generally performs well in most cases (and you have the option of both optimal parse and fast compression). LZSSE2 is recommended if you are only using text, especially heavily compressible text, but is slow/doesn't compress as well on less compressible data and binaries.

The code is approaching production readiness and LZSSE2 and LZSSE8 have received a reasonable amount of testing.

See these blog posts [An LZ Codec Designed for SSE Decompression](http://conorstokes.github.io/compression/2016/02/15/an-LZ-codec-designed-for-SSE-decompression) and [Compressor Improvements and LZSSE2 vs LZSSE8](http://conorstokes.github.io/compression/2016/02/24/compressor-improvements-and-lzsse2-vs-lzsse8) for a description of how the compression algorithm and implementation function. There are also benchmarks, but these may not be upto date (in particular the figures in the initial blog post no longer represent compression performance).