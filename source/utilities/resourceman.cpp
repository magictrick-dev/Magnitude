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

    if (ResourceManager::instance == nullptr)
    {
        ResourceManager* ins = new ResourceManager();
        std::shared_ptr<ResourceManager> created_rman(ins);
        ResourceManager::instance = created_rman;
    }

    return *ResourceManager::instance;

}

ResourceManager::
ResourceManager()
{

}

