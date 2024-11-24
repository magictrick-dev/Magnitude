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
                    FileResource(i64 index, Filepath path);
        virtual    ~FileResource();

        inline virtual bool     reserve()           override;
        inline virtual bool     load()              override;
        inline virtual bool     release()           override;
        inline virtual bool     resize(u64 size)    override;

};

FileResource::
FileResource(i64 index, Filepath path) : IResource(index)
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
// Represents resources that are in-memory only. When the user passes in their
// own buffer, it is *copied* so that the memory resource maintains ownership.
// Otherwise, the user is free to let the memory resource acquire its own memory
// using a different constructor.
//

class MemoryResource : public IResource
{

    public:
                    MemoryResource(i64 index, vptr buffer, u64 size);
        virtual    ~MemoryResource();

        inline virtual bool     reserve()           override;
        inline virtual bool     load()              override;
        inline virtual bool     release()           override;
        inline virtual bool     resize(u64 size)    override;

};

MemoryResource::
MemoryResource(i64 index, vptr buffer, u64 size) : IResource(index)
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

    // Search if the resource already exists.
    std::string path_as_string = path.c_str();
    auto result = self.resource_lookup.find(path_as_string);
    if (result != self.resource_lookup.end())
    {
        rhandle handle = result->second;
        return handle;
    }

    // Ensure the path is valid first.
    if (!path.is_valid_file()) return -1;

    // Otherwise the resource is added to the loopup table
    rhandle current_index = self.resource_list.size();
    std::shared_ptr<FileResource> res = std::make_shared<FileResource>(current_index, path);
    self.resource_list.push_back(res);
    self.resource_lookup[path_as_string] = current_index;
    return current_index;

}

rhandle ResourceManager::
create_memory_resource(vptr buffer, u64 buffer_size)
{

    ResourceManager& self = ResourceManager::get();
    rhandle current_index = self.resource_list.size();
    std::shared_ptr<MemoryResource> res = std::make_shared<MemoryResource>(
            current_index, buffer, buffer_size);
    self.resource_list.push_back(res);
    return current_index;

}

void ResourceManager::
release_resource(rhandle handle)
{

    ResourceManager& self = ResourceManager::get();
    std::shared_ptr<IResource> res = self.get_resource(handle);
    MAG_ENSURE_PTR(res);
    res->release();
    return;

}

bool ResourceManager::
resource_handle_is_valid(rhandle handle)
{

    ResourceManager& self = ResourceManager::get();
    if (handle < 0 || handle >= self.resource_list.size())
        return false;
    return true;

}
        
bool ResourceManager::
resource_is_reserved(rhandle handle)
{

    ResourceManager& self = ResourceManager::get();
    std::shared_ptr<IResource> res = self.get_resource(handle);
    MAG_ENSURE_PTR(res);
    b32 result = res->get_buffer() != nullptr;
    return result;

}

bool ResourceManager::
resource_is_loaded(rhandle handle)
{

    ResourceManager& self = ResourceManager::get();
    std::shared_ptr<IResource> res = self.get_resource(handle);
    MAG_ENSURE_PTR(res);
    b32 result = res->is_loaded();
    return result;

}

vptr ResourceManager::
get_resource_as_raw(rhandle handle)
{

    ResourceManager& self = ResourceManager::get();
    std::shared_ptr<IResource> res = self.get_resource(handle);
    MAG_ENSURE_PTR(res);
    vptr result = res->get_buffer();
    return result;

}

ccptr ResourceManager::
get_resource_as_string(rhandle handle)
{

    ResourceManager& self = ResourceManager::get();
    std::shared_ptr<IResource> res = self.get_resource(handle);
    MAG_ENSURE_PTR(res);
    ccptr result = (ccptr)res->get_buffer();
    return result;

}

cptr ResourceManager::
get_resource_as_string_buffer(rhandle handle)
{

    ResourceManager& self = ResourceManager::get();
    std::shared_ptr<IResource> res = self.get_resource(handle);
    MAG_ENSURE_PTR(res);
    cptr result = (cptr)res->get_buffer();
    return result;

}

u64 ResourceManager::
get_resource_size(rhandle handle)
{

    ResourceManager& self = ResourceManager::get();
    std::shared_ptr<IResource> res = self.get_resource(handle);
    MAG_ENSURE_PTR(res);
    u64 result = res->get_size();
    return result;

}

Filepath ResourceManager::
get_resource_file_path(rhandle handle)
{

    ResourceManager& self = ResourceManager::get();
    std::shared_ptr<IResource> res = self.get_resource(handle);
    MAG_ENSURE_PTR(res);
    Filepath result = res->get_path();
    return result;

}

bool ResourceManager::
reserve_resource(rhandle handle)
{

    ResourceManager& self = ResourceManager::get();
    std::shared_ptr<IResource> res = self.get_resource(handle);
    MAG_ENSURE_PTR(res);
    bool result = res->reserve();
    return result;

}

bool ResourceManager::
load_resource(rhandle handle)
{

    ResourceManager& self = ResourceManager::get();
    std::shared_ptr<IResource> res = self.get_resource(handle);
    MAG_ENSURE_PTR(res);
    bool result = res->load();
    return result;

}

bool ResourceManager::
resize_resource(rhandle handle, u64 new_size)
{

    ResourceManager& self = ResourceManager::get();
    std::shared_ptr<IResource> res = self.get_resource(handle);
    MAG_ENSURE_PTR(res);
    bool result = res->resize(new_size);
    return result;

}

std::shared_ptr<IResource> ResourceManager::
get_resource(rhandle handle)
{

    // Resource doesn't exist in this case.
    if (handle < 0 || handle >= this->resource_list.size())
        return nullptr;

    // Fetch and check.
    std::shared_ptr<IResource> res = this->resource_list[handle];
    MAG_ASSERT(res->get_handle_index() == handle && 
            "Did you accidently increment the handle when you shouldn't have?");

    return res;

}
