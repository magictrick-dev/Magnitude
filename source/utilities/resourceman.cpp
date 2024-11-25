#include <utilities/resourceman.hpp>
#include <platform/system.hpp>
#include <platform/filesystem.hpp>

// --- File Resource -----------------------------------------------------------
//
// Represents resources that use files. File resources don't resize so any requests
// to resize them doesn't actually evaluate to anything.
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

    MAG_ASSERT(path.is_valid_file() &&
            "This should be checked *before* you attempt to generate the resource.");

    this->path          = path;
    this->buffer_ptr    = nullptr;
    this->buffer_size   = 0;
    this->loaded        = false;

}

FileResource::
~FileResource()
{

    this->release();

}

bool FileResource::
reserve()
{

    u64 fsize = file_size(this->path.c_str());
    vptr buffer = system_virtual_alloc(NULL, fsize + 1);
    MAG_ASSERT(buffer != nullptr && "Virtual alloc shouldn't fail... but it did!");
    u64 actual_size = system_virtual_buffer_size(buffer);
    this->buffer_ptr = buffer;
    this->buffer_size = actual_size;
    memset(this->buffer_ptr, '\0', this->buffer_size); // Zip to zero.
    return true;

}

bool FileResource::
load()
{

    if (this->buffer_ptr == nullptr) MAG_ASSERT(this->reserve());
    u64 resulting_size = file_size(this->path.c_str());
    u64 read = file_read_all(this->path.c_str(), this->buffer_ptr, this->buffer_size);
    MAG_ASSERT(resulting_size == read && "The read chunk size should the same as the file size.");
    *((cptr)this->buffer_ptr + read) = '\0';    // Ensure last is null-terminated in case
                                                // some weird edge-case shit happens.
    return true;

}

bool FileResource::
release()
{

    if (this->buffer_ptr)
    {

        system_virtual_free(this->buffer_ptr);
        this->buffer_ptr    = nullptr;
        this->buffer_size   = 0;
        this->loaded        = false;
        return true;

    }

    return false;

}

bool FileResource::
resize(u64 size)
{

    // Resizing isn't allowed.
    return false;

}

// --- Memory Resource ---------------------------------------------------------
//
// Represents resources that are in-memory only. When the user passes in their
// own buffer, it is *copied* so that the memory resource maintains ownership.
// Otherwise, the user is free to let the memory resource acquire its own memory
// using a different constructor.
//
// Memory resources are resizable, but are not reservable or loadable, they will
// automatically load themselves and be in a valid loaded state.
//

class MemoryResource : public IResource
{

    public:
                    MemoryResource(i64 index, u64 size);
                    MemoryResource(i64 index, vptr buffer, u64 size);
        virtual    ~MemoryResource();

        inline virtual bool     reserve()           override;
        inline virtual bool     load()              override;
        inline virtual bool     release()           override;
        inline virtual bool     resize(u64 size)    override;

};

MemoryResource::
MemoryResource(i64 index, u64 size) : IResource(index)
{

    vptr buffer = system_virtual_alloc(NULL, size);
    u64 actual_size = system_virtual_buffer_size(buffer);
    this->buffer_ptr = buffer;
    this->buffer_size = actual_size;
    this->loaded = true;

}

MemoryResource::
MemoryResource(i64 index, vptr buffer, u64 size) : IResource(index)
{

    vptr actual_buffer = system_virtual_alloc(NULL, size);
    u64 actual_size = system_virtual_buffer_size(actual_buffer);
    memcpy(actual_buffer, buffer, size);

    this->buffer_ptr = actual_buffer;
    this->buffer_size = actual_size;
    this->loaded = true;

}

MemoryResource::
~MemoryResource()
{

    this->release();

}

bool MemoryResource::
reserve()
{

    // No need to reserve.
    return false;

}

bool MemoryResource::
load()
{

    // No need to load.
    return false;

}

bool MemoryResource::
release()
{

    if (this->buffer_ptr)
    {

        system_virtual_free(this->buffer_ptr);
        this->buffer_ptr    = nullptr;
        this->buffer_size   = 0;
        this->loaded        = false;
        return true;

    }

    return false;

}

bool MemoryResource::
resize(u64 size)
{

    if (size >= this->buffer_size)
    {

        
        vptr actual_buffer = system_virtual_alloc(NULL, size);
        u64 actual_size = system_virtual_buffer_size(actual_buffer);
        memcpy(actual_buffer, this->buffer_ptr, this->buffer_size);

        this->buffer_ptr = actual_buffer;
        this->buffer_size = actual_size;
        this->loaded = true;

    }

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

rhandle ResourceManager::
create_memory_resource(u64 request_size)
{

    ResourceManager& self = ResourceManager::get();
    rhandle current_index = self.resource_list.size();
    std::shared_ptr<MemoryResource> res = std::make_shared<MemoryResource>(current_index, request_size);
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
