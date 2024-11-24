#include <utilities/resourceman.hpp>
#include <platform/system.hpp>
#include <platform/filesystem.hpp>

// --- File Resource -----------------------------------------------------------
//
// Represents resources that use files.
//

class FileResource : public IResource
{

    public:
                    FileResource();
        virtual    ~FileResource();

        inline virtual bool     reserve()           override;
        inline virtual bool     load()              override;
        inline virtual bool     release()           override;
        inline virtual bool     resize(u64 size)    override;

};

FileResource::
FileResource()
{

}

FileResource::
~FileResource()
{

}

bool FileResource::
reserve()
{

    return false;

}

bool FileResource::
load()
{

    return false;

}

bool FileResource::
release()
{

    return false;

}

bool FileResource::
resize(u64 size)
{

    return false;

}

// --- Memory Resource ---------------------------------------------------------
//
// Represents resources that are in-memory only.
//

class MemoryResource : public IResource
{

    public:
                    MemoryResource();
        virtual    ~MemoryResource();

        inline virtual bool     reserve()           override;
        inline virtual bool     load()              override;
        inline virtual bool     release()           override;
        inline virtual bool     resize(u64 size)    override;

};

MemoryResource::
MemoryResource()
{

}

MemoryResource::
~MemoryResource()
{

}

bool MemoryResource::
reserve()
{

    return false;

}

bool MemoryResource::
load()
{

    return false;

}

bool MemoryResource::
release()
{

    return false;

}

bool MemoryResource::
resize(u64 size)
{

    return false;

}


// --- Resource Manager Implementation -----------------------------------------
//
// Pretty self-explanatory here, it's an internal singleton thingy or whatever.
// Essentially, we use handles that we pass around like hot cakes and those are
// fed to the API for use.
//

ResourceManager& ResourceManager::
get()
{

    static std::shared_ptr<ResourceManager> instance = nullptr;

    if (instance == nullptr)
    {
        ResourceManager *manager = new ResourceManager();
        instance = std::shared_ptr<ResourceManager>(manager);
    }

    return *instance;

}

ResourceManager::
ResourceManager()
{

}

ResourceManager::
~ResourceManager()
{

}

rhandle ResourceManager::
create_file_resource(Filepath path)
{

    ResourceManager& self = ResourceManager::get();

    return -1;
}

rhandle ResourceManager::
create_memory_resource(vptr buffer, u64 buffer_size)
{

    ResourceManager& self = ResourceManager::get();

    return -1;

}

void ResourceManager::
release_resource(rhandle handle)
{

    ResourceManager& self = ResourceManager::get();

    return;

}

bool ResourceManager::
resource_handle_is_valid(rhandle handle)
{

    ResourceManager& self = ResourceManager::get();

    return false;

}
        
bool ResourceManager::
resource_is_reserved(rhandle handle)
{

    ResourceManager& self = ResourceManager::get();

    return false;

}

bool ResourceManager::
resource_is_loaded(rhandle handle)
{

    ResourceManager& self = ResourceManager::get();

    return false;

}

bool ResourceManager::
resource_is_memory_resource(rhandle handle)
{

    ResourceManager& self = ResourceManager::get();

    return false;

}

vptr ResourceManager::
get_resource_as_raw(rhandle handle)
{

    ResourceManager& self = ResourceManager::get();

    return nullptr;

}

ccptr ResourceManager::
get_resource_as_string(rhandle handle)
{

    ResourceManager& self = ResourceManager::get();

    return nullptr;

}

cptr ResourceManager::
get_resource_as_string_buffer(rhandle handle)
{

    ResourceManager& self = ResourceManager::get();

    return nullptr;

}

u64 ResourceManager::
get_resource_size(rhandle handle)
{

    ResourceManager& self = ResourceManager::get();

    return 0;

}

Filepath ResourceManager::
get_resource_file_path(rhandle handle)
{

    ResourceManager& self = ResourceManager::get();

    return {};

}

bool ResourceManager::
reserve_resource(rhandle handle)
{

    ResourceManager& self = ResourceManager::get();

    return false;

}

bool ResourceManager::
load_resource(rhandle handle)
{

    ResourceManager& self = ResourceManager::get();

    return false;

}

bool ResourceManager::
resize_resource(rhandle handle, u64 new_size)
{

    ResourceManager& self = ResourceManager::get();

    return false;

}
