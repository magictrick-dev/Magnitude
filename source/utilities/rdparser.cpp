#include <utilities/rdparser.hpp>

// --- Parser Implementation ---------------------------------------------------
//
// I've gotten pretty good at doing this, believe it or not.
//

RDSyntaxParser::
RDSyntaxParser()
{

    return;
}

RDSyntaxParser::
RDSyntaxParser(Filepath path)
{

    this->reset(path);
    return;

}

RDSyntaxParser::
RDSyntaxParser(std::string memory_resource)
{

    this->reset(memory_resource);
    return;

}

RDSyntaxParser::
~RDSyntaxParser()
{

    return;

}

void RDSyntaxParser::
reset(Filepath path)
{
    this->tokenizer.reset(path);
    this->nodes.clear();
    this->root = nullptr;
    return;
}

void RDSyntaxParser::
reset(std::string memory_resource)
{

    this->tokenizer.reset(memory_resource);
    this->nodes.clear();
    this->root = nullptr;
    return;

}

shared_ptr<RDSyntaxNodeAbstract> RDSyntaxParser::
match_root()
{

    return nullptr;
}

shared_ptr<RDSyntaxNodeAbstract> RDSyntaxParser::
match_display()
{

    return nullptr;
}

shared_ptr<RDSyntaxNodeAbstract> RDSyntaxParser::
match_frame()
{

    return nullptr;
}

shared_ptr<RDSyntaxNodeAbstract> RDSyntaxParser::
match_frame_parameters()
{

    return nullptr;
}

shared_ptr<RDSyntaxNodeAbstract> RDSyntaxParser::
match_world()
{

    return nullptr;
}

shared_ptr<RDSyntaxNodeAbstract> RDSyntaxParser::
match_world_parameters()
{

    return nullptr;
}


bool RDSyntaxParser::
construct_ast()
{

    return false;
}

bool RDSyntaxParser::
is_valid() const
{

    bool result = (root != nullptr);
    return result;

}

void RDSyntaxParser::
synchronize_to(RDViewTokenType type)
{

    RDViewToken current_token = this->tokenizer.get_current();
    while (current_token.type != type || 
           current_token.type != RDViewTokenType::TypeEOF)
    {
        this->tokenizer.shift();
        current_token = this->tokenizer.get_current();
    }

    return;

}


