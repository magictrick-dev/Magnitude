#ifndef MAGNITUDE_UTILITIES_RESOURCEMAN_HPP
#define MAGNITUDE_UTILITIES_RESOURCEMAN_HPP
#include <unordered_map>
#include <string>
#include <vector>
#include <definitions.hpp>
#include <utilities/path.hpp>

typedef int64_t rhandle;

// --- Resource Interface ------------------------------------------------------ 
//
// There are two primary types of resources currently:
//      1.  File resources that pulled from disk.
//      2.  In-memory resources that the user creates.
//
// The resource interface abstracts the method for pulling information to and
// from them. When accessing resource, it technically doesn't matter where it
// came from, just that it exists in a valid spot of memory.
//
// File resources are the classical types that you will interact with. Resources
// that are inserted into the manager are permanent. If you release a resource,
// you are effectively deleting the memory it points to, not the handle entry that
// was added to interact with it.
//
// Memory resources are basically the same thing as a file resource except they
// do not have a filepath associated with them. They have an empty path. As a
// consequence, when they are freed, they can't be reloaded. It's up to the user
// to properly handle this situation.
//

class IResource
{

    public:
        inline              IResource(i64 index) : associated_handle(index) { };
        inline virtual     ~IResource() { };

        inline Filepath     get_path() const            { return this->path; }
        inline vptr         get_buffer() const          { return this->buffer_ptr; }
        inline u64          get_size() const            { return this->buffer_size; }
        inline i64          get_handle_index() const    { return this->associated_handle; }
        inline b32          is_loaded() const           { return this->loaded; }

        virtual bool        reserve()           = 0;
        virtual bool        load()              = 0;
        virtual bool        release()           = 0;
        virtual bool        resize(u64 size)    = 0;

    protected:
        Filepath        path;
        vptr            buffer_ptr;
        u64             buffer_size;
        b32             loaded;
        rhandle         associated_handle;

};

// --- Resource Manager --------------------------------------------------------
//
// The resource manager is responsible for storing long-term assets into memory.
// Rather than dealing directly with file I/O in instances where we want to operate
// on data like rdview source files, models, textures, and the like, we use this
// interface to keep it around for as long as we need it.
//
// Singleton by design, since there only needs to be one resource manager to contain
// the state of all resources. Single threaded; therefore if we want to expand this
// across multiple threads, we will need some thread-synchronization and locking
// primitives for certain resources.
//
// TODO(Chris): Multithreading primitives in case we want to blow this out as some
//              super-duper fast resource API with asynchronous loading, streaming,
//              or whatever. It might be super cool to have.
//

class ResourceManager
{

    public:
        static rhandle      create_file_resource(Filepath path); 
        static rhandle      create_memory_resource(vptr buffer, u64 buffer_size);
        static void         release_resource(rhandle handle);

        static bool         resource_handle_is_valid(rhandle handle);
        static bool         resource_is_reserved(rhandle handle);
        static bool         resource_is_loaded(rhandle handle);
        static vptr         get_resource_as_raw(rhandle handle);
        static ccptr        get_resource_as_string(rhandle handle);
        static cptr         get_resource_as_string_buffer(rhandle handle);
        static u64          get_resource_size(rhandle handle);
        static Filepath     get_resource_file_path(rhandle handle);

        static bool         reserve_resource(rhandle handle);
        static bool         load_resource(rhandle handle);

        static bool         resize_resource(rhandle handle, u64 new_size);

        virtual            ~ResourceManager();
    protected:
        ResourceManager();

        std::shared_ptr<IResource> get_resource(rhandle handle);

        std::unordered_map<std::string, rhandle>    resource_lookup;
        std::vector<std::shared_ptr<IResource>>     resource_list;

    protected:
        static ResourceManager& get();

};

#endif
