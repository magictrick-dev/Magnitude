#ifndef MAGNITUDE_DEFINITIONS_H 
#define MAGNITUDE_DEFINITIONS_H 
#include <cstdint>
#include <cassert>

typedef uint8_t     u8;
typedef uint16_t    u16;
typedef uint32_t    u32;
typedef uint64_t    u64;
typedef int8_t      i8;
typedef int16_t     i16;
typedef int32_t     i32;
typedef int64_t     i64;
typedef int8_t      b8;
typedef int16_t     b16;
typedef int32_t     b32;
typedef int64_t     b64;
typedef float       r32;
typedef double      r64;
typedef const char* ccptr;
typedef char*       cptr;
typedef void*       vptr;

#define MAG_BYTES(n)         ((u64)n)
#define MAG_KILOBYTES(n)     (MAG_BYTES(n)        * 1024)
#define MAG_MEGABYTES(n)     (MAG_KILOBYTES(n)    * 1024)
#define MAG_GIGABYTES(n)     (MAG_MEGABYTES(n)    * 1024)
#define MAG_TERABYTES(n)     (MAG_GIGABYTES(n)    * 1024)

#define MAG_ASSERT(stm)      (assert(stm))
#define MAG_ENSURE_PTR(ptr)  (assert((ptr != NULL)))
#define MAG_NO_IMPL(msg)     (assert(!(msg)))

#define MAG_MEMORY_ALLOC(sz) (::malloc(sz))
#define MAG_MEMORY_FREE(ptr) (::free(ptr))

#endif
