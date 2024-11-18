#ifndef MAGNITUDE_PLATFORM_FILESYSTEM_H
#define MAGNITUDE_PLATFORM_FILESYSTEM_H
#include <definitions.hpp>

b32         file_exists(ccptr file_path);
b32         file_ready(ccptr file_path);
b32         file_is_directory(ccptr file_path);
b32         file_is_file(ccptr file_path);
u64         file_size(ccptr file_path);
u64         file_read_all(ccptr file_path, vptr buffer, u64 buffer_size);
u64         file_write_all(ccptr file_path, vptr buffer, u64 buffer_size);
u64         file_last_write_time(ccptr file_path);

u64         file_current_working_directory(u32 buffer_size, cptr buffer);
void        file_canonicalize_path(u32 buffer_size, cptr dest, ccptr path);

#endif
