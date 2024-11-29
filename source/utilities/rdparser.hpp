#ifndef MAGNITUDE_UTILITIES_RDPARSER_HPP
#define MAGNITUDE_UTILITIES_RDPARSER_HPP
#include <common.hpp>
#include <vector>
#include <utilities/rdtokenizer.hpp>

// --- RDViewer Abstract Syntax Node -------------------------------------------
//
// The abstract syntax node represents a node, un-typed from its derived class.
// As an interface class, its primary function is to be a traversable unit abstracted
// from its derived implementation. The most common way to do this is via a visitor
// interface common for this type of operation.
//

class RDSyntaxNodeAbstract;
class RDSyntaxNodeRoot;
class RDSyntaxNodeDisplay;
class RDSyntaxNodeWorld;
class RDSyntaxNodeFrame;
class RDSyntaxNodeCameraEye;
class RDSyntaxNodeCameraAt;
class RDSyntaxNodeCameraUp;
class RDSyntaxNodeCameraFOV;

class RDSyntaxVisitor
{
    public:
        virtual void visit_root_syntax_node(RDSyntaxNodeRoot *node)                 = 0;
        virtual void visit_display_syntax_node(RDSyntaxNodeDisplay *node)           = 0;
        virtual void visit_world_syntax_node(RDSyntaxNodeWorld *node)               = 0;
        virtual void visit_frame_syntax_node(RDSyntaxNodeFrame *node)               = 0;
        virtual void visit_camera_eye_syntax_node(RDSyntaxNodeCameraEye *node)      = 0;
        virtual void visit_camera_at_syntax_node(RDSyntaxNodeCameraAt *node)        = 0;
        virtual void visit_camera_up_syntax_node(RDSyntaxNodeCameraUp *node)        = 0;
        virtual void visit_camera_fov_syntax_node(RDSyntaxNodeCameraFOV *node)      = 0;
};

enum class RDSyntaxNodeType
{
    NodeTypeVoid,
    NodeTypeRoot,
    NodeTypeDisplay,
    NodeTypeWorld,
    NodeTypeFrame,
    NodeTypeCameraEye,
    NodeTypeCameraAt,
    NodeTypeCameraUp,
    NodeTypeCameraFOV,
};

class RDSyntaxNodeAbstract
{

    public:
        inline          RDSyntaxNodeAbstract();
        inline virtual ~RDSyntaxNodeAbstract();

        inline RDSyntaxNodeType     get_type() const { return this->type; }
        template <class T> inline T cast_to() { return dynamic_pointer_cast<T>(this); }

        virtual void    accept(RDSyntaxVisitor *visitor) = 0;

    protected:
        RDSyntaxNodeType type;

};

// --- Derived Syntax Nodes ----------------------------------------------------
//
// The all-encompassing list of nodes. As you might imagine, this list is going to
// get kinda long as the language gets larger.
//

// --- Parser ------------------------------------------------------------------
//
// Finally, the big-bad parser that you've been waiting for. Parsing is a semi-expensive
// operation. Therefore, when integrating this with the GUI system, the parser should
// only re-parse when there are changes to the actual source document. This prevents
// unnecessary frame-by-frame parsing.
//
// The "construct_ast()" method is your golden ticket to getting a parsed AST. The
// return value dictates whether or not the parse was successful or not. If the parse
// failed, it means the entire AST is non-traversable (meaning its missing critical
// components or the parser was unable to recover in some meaningful way). A value of
// true *does not* mean that the AST tree is fully valid. Some parts are, which are
// traversable and viewable, while others are not.
//
// In order to determine where an error occurs, you must request the list of errors.
// These can be examined and output back to the user. The language specification states
// that any tree is valid given that the headers are well-formed. A frame is considered
// valid if the entire frame parsed correctly. A frame is marked invalid if it failed to
// parse. Frames can be skipped to render other frames for debugging.
//
// In order to actually do anything meaningful with the tree, you need to traverse
// it. There are lots of ways to do that, but the easiest way to do is to write a
// visitor routine to act on the tree.
//

class RDSyntaxParser
{

    public:
                    RDSyntaxParser();
                    RDSyntaxParser(Filepath path);
                    RDSyntaxParser(std::string memory_resource);
        virtual    ~RDSyntaxParser();

        void        reset(Filepath path);
        void        reset(std::string memory_resource);

        bool        construct_ast();
        bool        is_valid() const;

    protected:
        void        synchronize_to(RDViewTokenType type);

        template <class T, class... Args> inline shared_ptr<T> generate_node(Args... args);

    protected:
        shared_ptr<RDSyntaxNodeAbstract> root;
        std::vector<shared_ptr<RDSyntaxNodeAbstract>> nodes;
        RDViewTokenizer tokenizer;

};

// Classic template magic.
template <class T, class... Args> shared_ptr<T> RDSyntaxParser::
generate_node(Args... args)
{
    shared_ptr<T> new_node = std::make_shared<T>(&args...);
    shared_ptr<RDSyntaxNodeAbstract> pushable = 
        dynamic_pointer_cast<RDSyntaxNodeAbstract>(new_node);
    this->nodes.push_back(pushable);
}

#endif
