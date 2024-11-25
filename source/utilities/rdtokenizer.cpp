#include <utilities/rdtokenizer.hpp>

// --- RDView Token ------------------------------------------------------------
//
// Essentially breaks down the object that was encountered into a higher order,
// digestable, unapologetic (and uncoupled) instance from the tokenizer itself.
//

RDViewToken::
RDViewToken()
{

}

RDViewToken::
RDViewToken(rhandle handle, i32 offset, i32 length)
{

}

RDViewToken::
~RDViewToken()
{

}

RDViewTokenType RDViewToken::
get_type() const
{

    return this->type;

}

void RDViewToken::
set_type(RDViewTokenType type)
{

    this->type = type;

}

i32 RDViewToken::
get_offset() const
{

    return this->offset;

}

i32 RDViewToken::
get_length() const
{

    return this->length;

}

std::pair<i32, i32> RDViewToken::
get_location() const
{

    return { 0, 0 };

}

Filepath RDViewToken::
get_filepath() const
{

    Filepath result = ResourceManager::get_resource_file_path(this->handle);
    return result;

}
