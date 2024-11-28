#ifndef MAGNITUDE_PLATFORM_FILESYSTEM_H
#define MAGNITUDE_PLATFORM_FILESYSTEM_H
#include <definitions.hpp>
#include <string>

// --- Filesystem --------------------------------------------------------------
//
// For all your file API needs, OS-abstracted.
//
// The "file_save_as_system_dialogue()" parameter "extension_filters" expects
// a list of extensions separated by a null-terminated. The last entry is double
// null-terminated (just like argv). Example "Excel XLSX File .xlsx\0*.xlsx\0", etc.
//
// TODO(Chris): The file_save_as_system_dialogue() function is basically a windows
//              exclusive format, so we will need to standardize the dialogue functions
//              so that it is more intuitive.
//

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

bool        file_confirm_message(ccptr header, ccptr message);
std::string file_open_system_dialogue();
std::string file_save_as_system_dialogue(ccptr default_extension, ccptr extension_filters);

#endif
