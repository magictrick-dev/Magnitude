#include <utilities/memory.hpp>

// --- Memory Arena ------------------------------------------------------------
//
// Definitions here-in. A lot of assertions here to catch unscrupulous behavior.
//

void    
memory_arena_initialize(memory_arena *arena, memory_buffer *buffer)
{

    MAG_ENSURE_PTR(arena);
    MAG_ENSURE_PTR(buffer);
    MAG_ENSURE_PTR(buffer->ptr);
    MAG_ASSERT(buffer->size > 0);

    arena->buffer = *buffer;
    arena->commit_bottom = 0;
    arena->commit_top = 0;

}

vptr    
memory_arena_push_bottom(memory_arena *arena, u64 size, bool zero)
{

    MAG_ENSURE_PTR(arena);
    MAG_ENSURE_PTR(arena->buffer.ptr);

    u64 remainder = memory_arena_free_size(arena);
    MAG_ASSERT(remainder >= size);

    u8* offset = (u8*)arena->buffer.ptr + arena->commit_bottom;
    if (zero == true) memset(offset, 0x00, size);
    arena->commit_bottom += size;
    return offset;

}

vptr    
memory_arena_push_top(memory_arena *arena, u64 size, bool zero)
{

    MAG_ENSURE_PTR(arena);
    MAG_ENSURE_PTR(arena->buffer.ptr);

    u64 remainder = memory_arena_free_size(arena);
    MAG_ASSERT(remainder >= size);

    arena->commit_top += size;
    u8* offset = (u8*)arena->buffer.ptr + arena->buffer.size - arena->commit_top;
    if (zero == true) memset(offset, 0x00, size);
    return offset;

}

void    
memory_arena_pop_bottom(memory_arena *arena, u64 size, bool zero)
{

    MAG_ENSURE_PTR(arena);
    MAG_ENSURE_PTR(arena->buffer.ptr);
    MAG_ASSERT(arena->commit_bottom >= size);

    arena->commit_bottom -= size;
    if (zero == true)
    {
        u8* offset = (u8*)arena->buffer.ptr + arena->commit_bottom;
        memset(offset, 0x00, size);
    }

}

void    
memory_arena_pop_top(memory_arena *arena, u64 size, bool zero)
{
    
    MAG_ENSURE_PTR(arena);
    MAG_ENSURE_PTR(arena->buffer.ptr);
    MAG_ASSERT(arena->commit_top >= size);
    
    if (zero == true)
    {
        u8* offset = (u8*)arena->buffer.ptr + arena->buffer.size - arena->commit_top;
        memset(offset, 0x00, size);
    }

    arena->commit_top -= size;

}

void    
memory_arena_reinitialize(memory_arena *arena)
{

    MAG_ENSURE_PTR(arena);
    arena->commit_bottom = 0;
    arena->commit_top = 0;

}

u64     
memory_arena_commit_size(memory_arena *arena)
{

    MAG_ENSURE_PTR(arena);
    u64 result = arena->commit_top + arena->commit_bottom;
    return result;

}

u64     
memory_arena_free_size(memory_arena *arena)
{

    MAG_ENSURE_PTR(arena);
    u64 commit = memory_arena_commit_size(arena);
    u64 result = arena->buffer.size - commit;
    return result;

}


