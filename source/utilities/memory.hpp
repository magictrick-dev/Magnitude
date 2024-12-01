#ifndef MAGNITUDE_UTILITIES_MEMORY_HPP
#define MAGNITUDE_UTILITIES_MEMORY_HPP
#include <definitions.hpp>
#include <platform/system.hpp>
#include <thread>

typedef struct memory_buffer
{
    vptr ptr;
    u64 size;
} memory_buffer;

// --- Memory Context ----------------------------------------------------------
//
// Provides a simple interface to set and use different kinds of memory allocation
// schemes through a singular entry point. The basic idea is that memory allocators
// can be pushed and popped from the current context. This allows certain functions
// to choose how memory is allocated. In general terms, it allows the user to swap
// from the dynamic allocator to a custom allocator without having to maintain it
// through each call and use the primary alloc/free calls to interact with them.
//
// 1.   The base memory context is just the malloc()/free() interface wrapper.
//      This wrapper is designed only to track the total amount allocated and free'd
//      by the system.
//
// 2.   Each allocator must inherit the IAllocator interface. This provides the
//      base set of functionality required by the memory context to serve allocations
//      to the front-end.
//
// 3.   Each memory context is thread local, so if another thread is created it
//      uses a different memory context. This is to ensure no contents are made
//      between other threads for memory.
//
// When an allocator gets brought in and out of context, the on_push/on_pop callbacks
// are issued to the interface. This allows you to create allocators that perform
// cleanup or startup routines.
//

class MemoryContext
{

    public:

    protected:

};

class IAllocator
{

    public:
        virtual vptr        allocate(u64 size)                  = 0;
        virtual void        release(vptr ptr)                   = 0;

        virtual void        on_push_context()                   = 0;
        virtual void        on_pop_context()                    = 0;

        virtual u64         get_allocation_calls_count()        = 0;
        virtual u64         get_release_calls_count()           = 0;
        virtual u64         get_allocation_amount()             = 0;
        virtual u64         get_release_amount()                = 0;

};

// --- Memory Arena ------------------------------------------------------------
//
// The memory arena is a classic stack allocator used for high performance memory
// allocation scenarios that benefit from bulk deallocation. Generally, stack
// allocators work great in areas where the useable memory region is wiped at
// every iteration of a loop (IE a game's runtime loop) or during temporary
// resource allocation (such as fetching textures). 
//
// This isn't really a replacement for dynamic allocators, but it's perfect for
// most cases where dynamic allocations aren't a hard requirement.
//
// The push and pop functions optionally allow the user to zero out memory. Default
// is false, since it isn't particularly useful outside of guaranteeing that the
// region is zero-initialized for situations that need that.
//

typedef struct memory_arena
{
    memory_buffer buffer;
    u64 commit_bottom;
    u64 commit_top;
} memory_arena;

void    memory_arena_initialize(memory_arena *arena, memory_buffer *buffer);
vptr    memory_arena_push_bottom(memory_arena *arena, u64 size, bool zero = false);
vptr    memory_arena_push_top(memory_arena *arena, u64 size, bool zero = false);
void    memory_arena_pop_bottom(memory_arena *arena, u64 size, bool zero = false);
void    memory_arena_pop_top(memory_arena *arena, u64 size, bool zero = false);
void    memory_arena_reinitialize(memory_arena *arena);
u64     memory_arena_commit_size(memory_arena *arena);
u64     memory_arena_free_size(memory_arena *arena);

#endif
