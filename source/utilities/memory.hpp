#ifndef MAGNITUDE_UTILITIES_MEMORY_HPP
#define MAGNITUDE_UTILITIES_MEMORY_HPP
#include <definitions.hpp>
#include <platform/system.hpp>

typedef struct memory_buffer
{
    vptr ptr;
    u64 size;
} memory_buffer;

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
