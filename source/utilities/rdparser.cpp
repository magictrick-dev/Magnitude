#include <utilities/rdparser.hpp>
#include <utilities/logging.hpp>
#include <exception>

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
    while (this->tokenizer.get_current_token_type() == RDViewTokenType::TypeKeyFrameBegin)
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
    if (frame_token.type != RDViewTokenType::TypeInteger)
    {
        this->display_error(frame_token, RDViewTokenType::TypeInteger);
        return nullptr;
    }
    this->tokenizer.shift();

    // Frame width.
    RDViewToken width_token = this->tokenizer.get_current_token();
    if (width_token.type != RDViewTokenType::TypeInteger)
    {
        this->display_error(width_token, RDViewTokenType::TypeInteger);
        return nullptr;
    }
    this->tokenizer.shift();

    // Frame height.
    RDViewToken height_token = this->tokenizer.get_current_token();
    if (height_token.type != RDViewTokenType::TypeInteger)
    {
        this->display_error(height_token, RDViewTokenType::TypeInteger);
        return nullptr;
    }
    this->tokenizer.shift();


    i32 f,w,h;
    if (!this->convert_to_i32(&f, frame_token.reference))
    {
        this->display_conversion_error(frame_token);
        return nullptr;
    }
    if (!this->convert_to_i32(&w, width_token.reference))
    {
        this->display_conversion_error(width_token);
        return nullptr;
    }
    if (!this->convert_to_i32(&h, height_token.reference))
    {
        this->display_conversion_error(height_token);
        return nullptr;
    }

    // Generate the display node.
    auto display_node = this->generate_node<RDSyntaxNodeDisplay>();
    display_node->title = name_token.reference;
    display_node->frames_per_second = f;
    display_node->width = w;
    display_node->height = h;
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

    i32 f;
    if (!this->convert_to_i32(&f, frame_number.reference))
    {
        this->display_conversion_error(frame_number);
        this->synchronize_to(RDViewTokenType::TypeKeyFrameEnd);
        return nullptr;
    }

    // Construct the frame node.
    auto frame_node = this->generate_node<RDSyntaxNodeFrame>();
    frame_node->index = f;
    frame_node->world = world;
    frame_node->parameters = frame_parameters;
    return frame_node;

}

shared_ptr<RDSyntaxNodeAbstract> RDSyntaxParser::
match_frame_parameters()
{

    switch (this->tokenizer.get_current_token_type())
    {

        case RDViewTokenType::TypeKeyCameraEye:     return this->match_camera_eye();
        case RDViewTokenType::TypeKeyCameraAt:      return this->match_camera_at();
        case RDViewTokenType::TypeKeyCameraUp:      return this->match_camera_up();
        case RDViewTokenType::TypeKeyCameraFOV:     return this->match_camera_fov();
        case RDViewTokenType::TypeKeyClipping:      return this->match_clipping();

    }

    return nullptr;

}

shared_ptr<RDSyntaxNodeAbstract> RDSyntaxParser::
match_camera_at()
{

    RDViewTokenType current_type = this->tokenizer.get_current_token_type();
    if (current_type != RDViewTokenType::TypeKeyCameraAt)
    {
        RDViewToken current_token = this->tokenizer.get_current_token();
        this->display_error(current_token, RDViewTokenType::TypeKeyCameraAt);
        return nullptr;
    }
    this->tokenizer.shift();

    // Match x.
    RDViewToken coord_x = this->tokenizer.get_current_token();
    if (coord_x.type != RDViewTokenType::TypeInteger &&
        coord_x.type != RDViewTokenType::TypeReal)
    {
        this->display_error(coord_x, RDViewTokenType::TypeReal);
        this->synchronize_to(RDViewTokenType::TypeKeyFrameEnd);
        return nullptr;
    }
    this->tokenizer.shift();

    // Match y.
    RDViewToken coord_y = this->tokenizer.get_current_token();
    if (coord_y.type != RDViewTokenType::TypeInteger &&
        coord_y.type != RDViewTokenType::TypeReal)
    {
        this->display_error(coord_y, RDViewTokenType::TypeReal);
        this->synchronize_to(RDViewTokenType::TypeKeyFrameEnd);
        return nullptr;
    }
    this->tokenizer.shift();

    // Match z.
    RDViewToken coord_z = this->tokenizer.get_current_token();
    if (coord_z.type != RDViewTokenType::TypeInteger &&
        coord_z.type != RDViewTokenType::TypeReal)
    {
        this->display_error(coord_z, RDViewTokenType::TypeReal);
        this->synchronize_to(RDViewTokenType::TypeKeyFrameEnd);
        return nullptr;
    }
    this->tokenizer.shift();

    vec3 coords;
    if (!this->convert_to_r32(&coords.x, coord_x.reference))
    {
        this->display_conversion_error(coord_x);
        this->synchronize_to(RDViewTokenType::TypeKeyFrameEnd);
        return nullptr;
    }

    if (!this->convert_to_r32(&coords.y, coord_y.reference))
    {
        this->display_conversion_error(coord_y);
        this->synchronize_to(RDViewTokenType::TypeKeyFrameEnd);
        return nullptr;
    }

    if (!this->convert_to_r32(&coords.z, coord_z.reference))
    {
        this->display_conversion_error(coord_z);
        this->synchronize_to(RDViewTokenType::TypeKeyFrameEnd);
        return nullptr;
    }

    auto camera_at_node = this->generate_node<RDSyntaxNodeCameraAt>();
    camera_at_node->coordinates = coords;

    return camera_at_node;

}

shared_ptr<RDSyntaxNodeAbstract> RDSyntaxParser::
match_camera_eye()
{

    RDViewTokenType current_type = this->tokenizer.get_current_token_type();
    if (current_type != RDViewTokenType::TypeKeyCameraEye)
    {
        RDViewToken current_token = this->tokenizer.get_current_token();
        this->display_error(current_token, RDViewTokenType::TypeKeyCameraEye);
        return nullptr;
    }
    this->tokenizer.shift();

    // Match x.
    RDViewToken coord_x = this->tokenizer.get_current_token();
    if (coord_x.type != RDViewTokenType::TypeInteger &&
        coord_x.type != RDViewTokenType::TypeReal)
    {
        this->display_error(coord_x, RDViewTokenType::TypeReal);
        this->synchronize_to(RDViewTokenType::TypeKeyFrameEnd);
        return nullptr;
    }
    this->tokenizer.shift();

    // Match y.
    RDViewToken coord_y = this->tokenizer.get_current_token();
    if (coord_y.type != RDViewTokenType::TypeInteger &&
        coord_y.type != RDViewTokenType::TypeReal)
    {
        this->display_error(coord_y, RDViewTokenType::TypeReal);
        this->synchronize_to(RDViewTokenType::TypeKeyFrameEnd);
        return nullptr;
    }
    this->tokenizer.shift();

    // Match z.
    RDViewToken coord_z = this->tokenizer.get_current_token();
    if (coord_z.type != RDViewTokenType::TypeInteger &&
        coord_z.type != RDViewTokenType::TypeReal)
    {
        this->display_error(coord_z, RDViewTokenType::TypeReal);
        this->synchronize_to(RDViewTokenType::TypeKeyFrameEnd);
        return nullptr;
    }
    this->tokenizer.shift();

    vec3 coords;
    if (!this->convert_to_r32(&coords.x, coord_x.reference))
    {
        this->display_conversion_error(coord_x);
        this->synchronize_to(RDViewTokenType::TypeKeyFrameEnd);
        return nullptr;
    }

    if (!this->convert_to_r32(&coords.y, coord_y.reference))
    {
        this->display_conversion_error(coord_y);
        this->synchronize_to(RDViewTokenType::TypeKeyFrameEnd);
        return nullptr;
    }

    if (!this->convert_to_r32(&coords.z, coord_z.reference))
    {
        this->display_conversion_error(coord_z);
        this->synchronize_to(RDViewTokenType::TypeKeyFrameEnd);
        return nullptr;
    }

    auto camera_eye_node = this->generate_node<RDSyntaxNodeCameraEye>();
    camera_eye_node->coordinates = coords;
    return camera_eye_node;

}

shared_ptr<RDSyntaxNodeAbstract> RDSyntaxParser::
match_camera_up()
{

    RDViewTokenType current_type = this->tokenizer.get_current_token_type();
    if (current_type != RDViewTokenType::TypeKeyCameraUp)
    {
        RDViewToken current_token = this->tokenizer.get_current_token();
        this->display_error(current_token, RDViewTokenType::TypeKeyCameraUp);
        return nullptr;
    }
    this->tokenizer.shift();

    // Match x.
    RDViewToken coord_x = this->tokenizer.get_current_token();
    if (coord_x.type != RDViewTokenType::TypeInteger &&
        coord_x.type != RDViewTokenType::TypeReal)
    {
        this->display_error(coord_x, RDViewTokenType::TypeReal);
        this->synchronize_to(RDViewTokenType::TypeKeyFrameEnd);
        return nullptr;
    }
    this->tokenizer.shift();

    // Match y.
    RDViewToken coord_y = this->tokenizer.get_current_token();
    if (coord_y.type != RDViewTokenType::TypeInteger &&
        coord_y.type != RDViewTokenType::TypeReal)
    {
        this->display_error(coord_y, RDViewTokenType::TypeReal);
        this->synchronize_to(RDViewTokenType::TypeKeyFrameEnd);
        return nullptr;
    }
    this->tokenizer.shift();

    // Match z.
    RDViewToken coord_z = this->tokenizer.get_current_token();
    if (coord_z.type != RDViewTokenType::TypeInteger &&
        coord_z.type != RDViewTokenType::TypeReal)
    {
        this->display_error(coord_z, RDViewTokenType::TypeReal);
        this->synchronize_to(RDViewTokenType::TypeKeyFrameEnd);
        return nullptr;
    }
    this->tokenizer.shift();

    vec3 coords;
    if (!this->convert_to_r32(&coords.x, coord_x.reference))
    {
        this->display_conversion_error(coord_x);
        this->synchronize_to(RDViewTokenType::TypeKeyFrameEnd);
        return nullptr;
    }

    if (!this->convert_to_r32(&coords.y, coord_y.reference))
    {
        this->display_conversion_error(coord_y);
        this->synchronize_to(RDViewTokenType::TypeKeyFrameEnd);
        return nullptr;
    }

    if (!this->convert_to_r32(&coords.z, coord_z.reference))
    {
        this->display_conversion_error(coord_z);
        this->synchronize_to(RDViewTokenType::TypeKeyFrameEnd);
        return nullptr;
    }

    auto camera_up_node = this->generate_node<RDSyntaxNodeCameraUp>();
    camera_up_node->coordinates = coords;
    return camera_up_node;

}

shared_ptr<RDSyntaxNodeAbstract> RDSyntaxParser::
match_camera_fov()
{

    RDViewTokenType current_type = this->tokenizer.get_current_token_type();
    if (current_type != RDViewTokenType::TypeKeyCameraFOV)
    {
        RDViewToken current_token = this->tokenizer.get_current_token();
        this->display_error(current_token, RDViewTokenType::TypeKeyCameraFOV);
        return nullptr;
    }
    this->tokenizer.shift();

    // Match FOV entry.
    RDViewToken fov = this->tokenizer.get_current_token();
    if (fov.type != RDViewTokenType::TypeInteger &&
        fov.type != RDViewTokenType::TypeReal)
    {
        this->display_error(fov, RDViewTokenType::TypeReal);
        this->synchronize_to(RDViewTokenType::TypeKeyFrameEnd);
        return nullptr;
    }
    this->tokenizer.shift();

    r32 n;
    if (!this->convert_to_r32(&n, fov.reference))
    {
        this->display_conversion_error(fov);
        this->synchronize_to(RDViewTokenType::TypeKeyFrameEnd);
        return nullptr;
    }

    auto camera_fov_node = this->generate_node<RDSyntaxNodeCameraFOV>();
    camera_fov_node->field_of_view = n;

    return camera_fov_node;

}

shared_ptr<RDSyntaxNodeAbstract> RDSyntaxParser::
match_clipping()
{

    RDViewTokenType current_type = this->tokenizer.get_current_token_type();
    if (current_type != RDViewTokenType::TypeKeyClipping)
    {
        RDViewToken current_token = this->tokenizer.get_current_token();
        this->display_error(current_token, RDViewTokenType::TypeKeyClipping);
        return nullptr;
    }
    this->tokenizer.shift();

    // Match near.
    RDViewToken near = this->tokenizer.get_current_token();
    if (near.type != RDViewTokenType::TypeInteger &&
        near.type != RDViewTokenType::TypeReal)
    {
        this->display_error(near, RDViewTokenType::TypeReal);
        this->synchronize_to(RDViewTokenType::TypeKeyFrameEnd);
        return nullptr;
    }
    this->tokenizer.shift();

    // Match far.
    RDViewToken far = this->tokenizer.get_current_token();
    if (far.type != RDViewTokenType::TypeInteger &&
        far.type != RDViewTokenType::TypeReal)
    {
        this->display_error(near, RDViewTokenType::TypeReal);
        this->synchronize_to(RDViewTokenType::TypeKeyFrameEnd);
        return nullptr;
    }
    this->tokenizer.shift();

    r32 n, f;

    if (!this->convert_to_r32(&n, near.reference))
    {
        this->display_conversion_error(near);
        this->synchronize_to(RDViewTokenType::TypeKeyFrameEnd);
        return nullptr;
    }

    if (!this->convert_to_r32(&f, far.reference))
    {
        this->display_conversion_error(far);
        this->synchronize_to(RDViewTokenType::TypeKeyFrameEnd);
        return nullptr;
    }

    auto clipping_node = this->generate_node<RDSyntaxNodeClipping>();
    clipping_node->near = n;
    clipping_node->far = f;

    return clipping_node;

}

shared_ptr<RDSyntaxNodeAbstract> RDSyntaxParser::
match_world()
{

    // Match WorldBegin
    RDViewTokenType current_type = this->tokenizer.get_current_token_type();
    if (current_type != RDViewTokenType::TypeKeyWorldBegin)
    {
        RDViewToken current_token = this->tokenizer.get_current_token();
        this->display_error(current_token, RDViewTokenType::TypeKeyWorldBegin);
        this->synchronize_to(RDViewTokenType::TypeKeyFrameEnd);
        return nullptr;
    }
    this->tokenizer.shift();

    // Match world parameters.
    std::vector<shared_ptr<RDSyntaxNodeAbstract>> world_parameters;
    shared_ptr<RDSyntaxNodeAbstract> current_parameter = nullptr;
    while (true)
    {

        // Match the world. Break if the world is returned null.
        current_parameter = this->match_world_parameters();
        if (current_parameter == nullptr) break;
        world_parameters.push_back(current_parameter);

    }

    // Match WorldEnd
    current_type = this->tokenizer.get_current_token_type();
    if (current_type != RDViewTokenType::TypeKeyWorldEnd)
    {
        RDViewToken current_token = this->tokenizer.get_current_token();
        this->display_error(current_token, RDViewTokenType::TypeKeyWorldEnd);
        this->synchronize_to(RDViewTokenType::TypeKeyFrameEnd);
        return nullptr;
    }
    this->tokenizer.shift();

    auto world_node = this->generate_node<RDSyntaxNodeWorld>();
    world_node->parameters = world_parameters;
    return world_node;


}

shared_ptr<RDSyntaxNodeAbstract> RDSyntaxParser::
match_world_parameters()
{

    switch (this->tokenizer.get_current_token_type())
    {

        case RDViewTokenType::TypeKeyPoint:     return this->match_point();

    }

    return nullptr;

}

shared_ptr<RDSyntaxNodeAbstract> RDSyntaxParser::
match_point()
{

    // Match point. 
    RDViewTokenType current_type = this->tokenizer.get_current_token_type();
    if (current_type != RDViewTokenType::TypeKeyPoint)
    {
        RDViewToken current_token = this->tokenizer.get_current_token();
        this->display_error(current_token, RDViewTokenType::TypeKeyPoint);
        this->synchronize_to(RDViewTokenType::TypeKeyWorldEnd);
        return nullptr;
    }
    this->tokenizer.shift();

    // Match x.
    RDViewToken coord_x = this->tokenizer.get_current_token();
    if (coord_x.type != RDViewTokenType::TypeInteger &&
        coord_x.type != RDViewTokenType::TypeReal)
    {
        this->display_error(coord_x, RDViewTokenType::TypeReal);
        this->synchronize_to(RDViewTokenType::TypeKeyWorldEnd);
        return nullptr;
    }
    this->tokenizer.shift();

    // Match y.
    RDViewToken coord_y = this->tokenizer.get_current_token();
    if (coord_y.type != RDViewTokenType::TypeInteger &&
        coord_y.type != RDViewTokenType::TypeReal)
    {
        this->display_error(coord_y, RDViewTokenType::TypeReal);
        this->synchronize_to(RDViewTokenType::TypeKeyWorldEnd);
        return nullptr;
    }
    this->tokenizer.shift();

    // Match z.
    RDViewToken coord_z = this->tokenizer.get_current_token();
    if (coord_z.type != RDViewTokenType::TypeInteger &&
        coord_z.type != RDViewTokenType::TypeReal)
    {
        this->display_error(coord_z, RDViewTokenType::TypeReal);
        this->synchronize_to(RDViewTokenType::TypeKeyWorldEnd);
        return nullptr;
    }
    this->tokenizer.shift();
    
    // Match radius.
    RDViewToken radius = this->tokenizer.get_current_token();
    if (radius.type != RDViewTokenType::TypeInteger &&
        radius.type != RDViewTokenType::TypeReal)
    {
        this->display_error(radius, RDViewTokenType::TypeReal);
        this->synchronize_to(RDViewTokenType::TypeKeyWorldEnd);
        return nullptr;
    }
    this->tokenizer.shift();

    // Attempt to convert, we need to use exception handling to ensure it doesn't
    // implode when we do this.
    //
    // TODO(Chris): Actually, this is slow as hell and we want to ensure this doesn't
    //              occur via except handling. The solution, of course, is to do check
    //              if it is within integer bounds.
    vec3 coords;
    r32 r;

    if (!this->convert_to_r32(&coords.x, coord_x.reference))
    {
        this->display_conversion_error(coord_x);
        this->synchronize_to(RDViewTokenType::TypeKeyWorldEnd);
        return nullptr;
    }

    if (!this->convert_to_r32(&coords.y, coord_y.reference))
    {
        this->display_conversion_error(coord_y);
        this->synchronize_to(RDViewTokenType::TypeKeyWorldEnd);
        return nullptr;
    }

    if (!this->convert_to_r32(&coords.z, coord_z.reference))
    {
        this->display_conversion_error(coord_z);
        this->synchronize_to(RDViewTokenType::TypeKeyWorldEnd);
        return nullptr;
    }

    if (!this->convert_to_r32(&r, radius.reference))
    {
        this->display_conversion_error(radius);
        this->synchronize_to(RDViewTokenType::TypeKeyWorldEnd);
        return nullptr;
    }

    auto point_node = this->generate_node<RDSyntaxNodePoint>();
    point_node->coordinates = coords;
    point_node->radius = r; 

    return point_node;

}

bool RDSyntaxParser::
construct_ast()
{

    auto root_node = this->match_root();
    if (root_node == nullptr) return false;
    this->root = root_node;
    return true;

}

void RDSyntaxParser::
visit_root(RDSyntaxVisitor *visitor)
{

    if (this->root != nullptr) this->root->accept(visitor);

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
    while (current_type != type &&
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

void RDSyntaxParser::
display_conversion_error(RDViewToken what)
{

    Logger::log_error(LogFlag_Parser, "Unable to convert %s at (%i, %i), value too large to parse.",
            to_string(what.type).c_str(), what.row, what.column);

}


bool RDSyntaxParser::      
convert_to_i32(i32 *in, std::string reference)
{

    try
    {
        *in = std::stoi(reference);
        return true;
    }
    catch (...)
    {
        return false;
    }

}

bool RDSyntaxParser::
convert_to_r32(r32 *in, std::string reference)
{

    try
    {
        *in = std::stof(reference);
        return true;
    }
    catch (...)
    {
        return false;
    }

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

// --- Syntax Node World -------------------------------------------------------
//
//
//

RDSyntaxNodeWorld::
RDSyntaxNodeWorld()
{
    this->type = RDSyntaxNodeType::NodeTypeWorld;
}

RDSyntaxNodeWorld::
~RDSyntaxNodeWorld()
{

}

// --- Syntax Node CameraEye ---------------------------------------------------
//
//
//

RDSyntaxNodeCameraEye::
RDSyntaxNodeCameraEye()
{
    this->type = RDSyntaxNodeType::NodeTypeCameraEye;
}

RDSyntaxNodeCameraEye::
~RDSyntaxNodeCameraEye()
{

}

// --- Syntax Node CameraAt ----------------------------------------------------
//
//
//

RDSyntaxNodeCameraAt::
RDSyntaxNodeCameraAt()
{
    this->type = RDSyntaxNodeType::NodeTypeCameraAt;
}

RDSyntaxNodeCameraAt::
~RDSyntaxNodeCameraAt()
{

}

// --- Syntax Node CameraUp ----------------------------------------------------
//
//
//

RDSyntaxNodeCameraUp::
RDSyntaxNodeCameraUp()
{
    this->type = RDSyntaxNodeType::NodeTypeCameraUp;
}

RDSyntaxNodeCameraUp::
~RDSyntaxNodeCameraUp()
{

}

// --- Syntax Node CameraFOV ----------------------------------------------------
//
//
//

RDSyntaxNodeCameraFOV::
RDSyntaxNodeCameraFOV()
{
    this->type = RDSyntaxNodeType::NodeTypeCameraFOV;
}

RDSyntaxNodeCameraFOV::
~RDSyntaxNodeCameraFOV()
{

}

// --- Syntax Node Clipping ----------------------------------------------------
//
//
//

RDSyntaxNodeClipping::
RDSyntaxNodeClipping()
{
    this->type = RDSyntaxNodeType::NodeTypeClipping;
}

RDSyntaxNodeClipping::
~RDSyntaxNodeClipping()
{

}

// --- Syntax Node Point ----------------------------------------------------
//
//
//

RDSyntaxNodePoint::
RDSyntaxNodePoint()
{
    this->type = RDSyntaxNodeType::NodeTypePoint;
}

RDSyntaxNodePoint::
~RDSyntaxNodePoint()
{

}

// --- Debug Traversal ---------------------------------------------------------
//
// The implementation for outputting the contents of the AST.
//

RDSyntaxOutputVisitor::
RDSyntaxOutputVisitor()
{

    this->tab_offset = 0;
    this->tab_size = 4;

}

RDSyntaxOutputVisitor::
~RDSyntaxOutputVisitor()
{

}

void RDSyntaxOutputVisitor::
visit_SyntaxNodeRoot(RDSyntaxNodeRoot *node)
{

    node->display_node->accept(this);
    for (auto frame : node->frames)
    {
        frame->accept(this);
    }

    std::string out = this->output_stream.str();
    Logger::log_debug(LogFlag_Parser, "Parser output:\n%s", out.c_str());

}

void RDSyntaxOutputVisitor::
visit_SyntaxNodeDisplay(RDSyntaxNodeDisplay *node)
{

    std::stringstream& ss = this->output_stream;
    ss  << "Display \"" << node->title << "\" "
        << node->frames_per_second << " "
        << node->width << " "
        << node->height << std::endl;

}

void RDSyntaxOutputVisitor::
visit_SyntaxNodeWorld(RDSyntaxNodeWorld *node)
{

    std::stringstream& ss = this->output_stream;
    ss  << "WorldBegin" << std::endl;

    for (auto parameter : node->parameters) parameter->accept(this);

    ss  << "WorldEnd" << std::endl;

}

void RDSyntaxOutputVisitor::
visit_SyntaxNodeFrame(RDSyntaxNodeFrame *node)
{

    std::stringstream& ss = this->output_stream;
    ss  << "FrameBegin " << node->index << std::endl;

    for (auto parameter : node->parameters) parameter->accept(this);
    node->world->accept(this);
    
    ss  << "FrameEnd" << std::endl;

}

void RDSyntaxOutputVisitor::
visit_SyntaxNodeCameraEye(RDSyntaxNodeCameraEye *node)
{

    std::stringstream& ss = this->output_stream;
    ss  << "CameraEye "
        << node->coordinates.x << " "
        << node->coordinates.y << " "
        << node->coordinates.z << std::endl;

}

void RDSyntaxOutputVisitor::
visit_SyntaxNodeCameraAt(RDSyntaxNodeCameraAt *node)
{

    std::stringstream& ss = this->output_stream;
    ss  << "CameraAt "
        << node->coordinates.x << " "
        << node->coordinates.y << " "
        << node->coordinates.z << std::endl;

}

void RDSyntaxOutputVisitor::
visit_SyntaxNodeCameraUp(RDSyntaxNodeCameraUp *node)
{

    std::stringstream& ss = this->output_stream;
    ss  << "CameraUp "
        << node->coordinates.x << " "
        << node->coordinates.y << " "
        << node->coordinates.z << std::endl;

}

void RDSyntaxOutputVisitor::
visit_SyntaxNodeCameraFOV(RDSyntaxNodeCameraFOV *node)
{

    std::stringstream& ss = this->output_stream;
    ss  << "CameraFOV " << node->field_of_view << std::endl;

}

void RDSyntaxOutputVisitor::
visit_SyntaxNodeClipping(RDSyntaxNodeClipping *node)
{
    std::stringstream& ss = this->output_stream;
    ss  << "Clipping " << node->near << " " << node->far << std::endl;
}

void RDSyntaxOutputVisitor::
visit_SyntaxNodePoint(RDSyntaxNodePoint *node)
{

    std::stringstream& ss = this->output_stream;
    ss  << "Point "
        << node->coordinates.x << " "
        << node->coordinates.y << " "
        << node->coordinates.z << " "
        << node->radius << std::endl;

}

