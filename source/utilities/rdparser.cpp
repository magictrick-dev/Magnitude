#include <utilities/rdparser.hpp>
#include <utilities/logging.hpp>

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

    // NOTE(Chris): The match root doesn't synchronize, any errors it encounters
    //              here is basically the parser saying *do nothing*.

    // Match display.
    shared_ptr<RDSyntaxNodeAbstract> display_node = this->match_display();
    if (display_node == nullptr) return nullptr;

    // Match all frames.
    std::vector<shared_ptr<RDSyntaxNodeAbstract>> frame_list;
    shared_ptr<RDSyntaxNodeAbstract> current_frame = nullptr;
    while (true)
    {

        // Match the frame. Break if the frame is returned null.
        current_frame = this->match_frame();
        if (current_frame == nullptr) break;
        frame_list.push_back(current_frame);

    }

    // Generate root node.
    auto root_node = this->generate_node<RDSyntaxNodeRoot>();
    root_node->display_node = display_node;
    root_node->frames = frame_list;
    return root_node;

}

shared_ptr<RDSyntaxNodeAbstract> RDSyntaxParser::
match_display()
{

    // Captures the Display token.
    RDViewTokenType current_type = this->tokenizer.get_current_token_type();
    if (current_type != RDViewTokenType::TypeKeyDisplay)
    {
        RDViewToken current_token = this->tokenizer.get_current_token();
        this->display_error(current_token, RDViewTokenType::TypeKeyDisplay);
        return nullptr;
    }
    this->tokenizer.shift();

    // Now the name of the project file.
    RDViewToken name_token = this->tokenizer.get_current_token();
    if (name_token.type != RDViewTokenType::TypeString)
    {
        this->display_error(name_token, RDViewTokenType::TypeString);
        return nullptr;
    }
    this->tokenizer.shift();

    // Frames per second.
    RDViewToken frame_token = this->tokenizer.get_current_token();
    if (name_token.type != RDViewTokenType::TypeInteger)
    {
        this->display_error(name_token, RDViewTokenType::TypeInteger);
        return nullptr;
    }
    this->tokenizer.shift();

    // Frame width.
    RDViewToken width_token = this->tokenizer.get_current_token();
    if (name_token.type != RDViewTokenType::TypeInteger)
    {
        this->display_error(name_token, RDViewTokenType::TypeInteger);
        return nullptr;
    }
    this->tokenizer.shift();

    // Frame height.
    RDViewToken height_token = this->tokenizer.get_current_token();
    if (name_token.type != RDViewTokenType::TypeInteger)
    {
        this->display_error(name_token, RDViewTokenType::TypeInteger);
        return nullptr;
    }
    this->tokenizer.shift();

    // Generate the display node.
    auto display_node = this->generate_node<RDSyntaxNodeDisplay>();
    display_node->title = name_token.reference;
    display_node->frames_per_second = std::stoi(frame_token.reference);
    display_node->width = std::stoi(width_token.reference);
    display_node->height = std::stoi(height_token.reference);
    return display_node;

}

shared_ptr<RDSyntaxNodeAbstract> RDSyntaxParser::
match_frame()
{

    // Match the frame.
    RDViewTokenType current_type = this->tokenizer.get_current_token_type();
    if (current_type != RDViewTokenType::TypeKeyFrameBegin)
    {
        RDViewToken current_token = this->tokenizer.get_current_token();
        this->display_error(current_token, RDViewTokenType::TypeKeyFrameBegin);
        return nullptr;
    }
    this->tokenizer.shift();

    // Get the frame number.
    RDViewToken frame_number = this->tokenizer.get_current_token();
    if (frame_number.type != RDViewTokenType::TypeInteger)
    {
        this->display_error(frame_number, RDViewTokenType::TypeInteger);
        return nullptr;
    }
    this->tokenizer.shift();
    
    // We can now match all frame parameters.
    std::vector<shared_ptr<RDSyntaxNodeAbstract>> frame_parameters;
    shared_ptr<RDSyntaxNodeAbstract> current_parameter = nullptr;
    while (true)
    {

        // Match the frame. Break if the frame is returned null.
        current_parameter = this->match_frame_parameters();
        if (current_parameter == nullptr) break;
        frame_parameters.push_back(current_parameter);

    }

    // Once we match all frame parameters, we can now match za wurldo
    shared_ptr<RDSyntaxNodeAbstract> world = this->match_world();

    // Finally, we check the FrameEnd.
    current_type = this->tokenizer.get_current_token_type();
    if (current_type != RDViewTokenType::TypeKeyFrameEnd)
    {
        RDViewToken current_token = this->tokenizer.get_current_token();
        this->display_error(current_token, RDViewTokenType::TypeKeyFrameEnd);
        return nullptr;
    }
    this->tokenizer.shift();

    // Construct the frame node.
    auto frame_node = this->generate_node<RDSyntaxNodeFrame>();
    frame_node->world = world;
    frame_node->parameters = frame_parameters;
    return frame_node;

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

    RDViewTokenType current_type = this->tokenizer.get_current_token_type();
    while (current_type != type || 
           current_type != RDViewTokenType::TypeEOF)
    {
        this->tokenizer.shift();
        current_type = this->tokenizer.get_current_token_type();
    }

    return;

}

void RDSyntaxParser::
display_error(RDViewToken what, RDViewTokenType expected)
{

    Logger::log_error(LogFlag_Parser, "Expected %s, encountered %s at (%i, %i).",
            to_string(expected).c_str(), to_string(what.type).c_str(), 
            what.row, what.column);

}

// --- Abstract Syntax Node ----------------------------------------------------
//
// The base abstract node type.
//

RDSyntaxNodeAbstract::
RDSyntaxNodeAbstract()
{

    type = RDSyntaxNodeType::NodeTypeVoid;

}

RDSyntaxNodeAbstract::
~RDSyntaxNodeAbstract()
{

}

// --- Root Syntax Node --------------------------------------------------------
//
// The root syntax node basically corresponds to the root of the AST and holds
// all necessary frames. This is the entry node.
//

RDSyntaxNodeRoot::
RDSyntaxNodeRoot()
{
    this->type = RDSyntaxNodeType::NodeTypeRoot;
}

RDSyntaxNodeRoot::
~RDSyntaxNodeRoot()
{

}

// --- Display Syntax Node -----------------------------------------------------
//
// The display parameters of the scene lie within.
//

RDSyntaxNodeDisplay::
RDSyntaxNodeDisplay()
{
    this->type = RDSyntaxNodeType::NodeTypeDisplay;
}

RDSyntaxNodeDisplay::
~RDSyntaxNodeDisplay()
{

}

// --- Frame Syntax Node -------------------------------------------------------
//
// The frame contains the world syntax node + some frame parameters.
//

RDSyntaxNodeFrame::
RDSyntaxNodeFrame()
{
    this->type = RDSyntaxNodeType::NodeTypeFrame;
}

RDSyntaxNodeFrame::
~RDSyntaxNodeFrame()
{

}
