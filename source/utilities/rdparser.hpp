#ifndef MAGNITUDE_UTILITIES_RDPARSER_HPP
#define MAGNITUDE_UTILITIES_RDPARSER_HPP
#include <common.hpp>
#include <vector>
#include <sstream>
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
class RDSyntaxNodeClipping;
class RDSyntaxNodePoint;

class RDSyntaxVisitor
{
    public:
        virtual void visit_SyntaxNodeRoot(RDSyntaxNodeRoot *node)               = 0;
        virtual void visit_SyntaxNodeDisplay(RDSyntaxNodeDisplay *node)         = 0;
        virtual void visit_SyntaxNodeWorld(RDSyntaxNodeWorld *node)             = 0;
        virtual void visit_SyntaxNodeFrame(RDSyntaxNodeFrame *node)             = 0;
        virtual void visit_SyntaxNodeCameraEye(RDSyntaxNodeCameraEye *node)     = 0;
        virtual void visit_SyntaxNodeCameraAt(RDSyntaxNodeCameraAt *node)       = 0;
        virtual void visit_SyntaxNodeCameraUp(RDSyntaxNodeCameraUp *node)       = 0;
        virtual void visit_SyntaxNodeCameraFOV(RDSyntaxNodeCameraFOV *node)     = 0;
        virtual void visit_SyntaxNodeClipping(RDSyntaxNodeClipping *node)       = 0;
        virtual void visit_SyntaxNodePoint(RDSyntaxNodePoint *node)             = 0;
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
    NodeTypeClipping,
    NodeTypePoint,
};

class RDSyntaxNodeAbstract
{

    public:
                    RDSyntaxNodeAbstract();
        virtual    ~RDSyntaxNodeAbstract();

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

class RDSyntaxNodeRoot : public RDSyntaxNodeAbstract
{
    public:
                        RDSyntaxNodeRoot();
        virtual        ~RDSyntaxNodeRoot();

        inline virtual void accept(RDSyntaxVisitor *visitor)
            { visitor->visit_SyntaxNodeRoot(this); }

        shared_ptr<RDSyntaxNodeAbstract> display_node;
        std::vector<shared_ptr<RDSyntaxNodeAbstract>> frames;

};

class RDSyntaxNodeDisplay : public RDSyntaxNodeAbstract
{
    public:
                        RDSyntaxNodeDisplay();
        virtual        ~RDSyntaxNodeDisplay();

        inline virtual void accept(RDSyntaxVisitor *visitor) override
            { visitor->visit_SyntaxNodeDisplay(this); }

        std::string title;
        i32 frames_per_second;
        i32 width;
        i32 height;

};

class RDSyntaxNodeFrame : public RDSyntaxNodeAbstract
{
    public:
                        RDSyntaxNodeFrame();
        virtual        ~RDSyntaxNodeFrame();

        inline virtual void accept(RDSyntaxVisitor *visitor) override
            { visitor->visit_SyntaxNodeFrame(this); }

        i32 index;
        shared_ptr<RDSyntaxNodeAbstract> world;
        std::vector<shared_ptr<RDSyntaxNodeAbstract>> parameters;

};

class RDSyntaxNodeWorld : public RDSyntaxNodeAbstract
{
    public:
                        RDSyntaxNodeWorld();
        virtual        ~RDSyntaxNodeWorld();

        inline virtual void accept(RDSyntaxVisitor *visitor) override
            { visitor->visit_SyntaxNodeWorld(this); }

        std::vector<shared_ptr<RDSyntaxNodeAbstract>> parameters;

};

class RDSyntaxNodeCameraEye : public RDSyntaxNodeAbstract
{
    public:
                        RDSyntaxNodeCameraEye();
        virtual        ~RDSyntaxNodeCameraEye();

        inline virtual void accept(RDSyntaxVisitor *visitor) override
            { visitor->visit_SyntaxNodeCameraEye(this); }

        vec3 coordinates;

};

class RDSyntaxNodeCameraAt : public RDSyntaxNodeAbstract
{
    public:
                        RDSyntaxNodeCameraAt();
        virtual        ~RDSyntaxNodeCameraAt();

        inline virtual void accept(RDSyntaxVisitor *visitor) override
            { visitor->visit_SyntaxNodeCameraAt(this); }

        vec3 coordinates;

};

class RDSyntaxNodeCameraUp : public RDSyntaxNodeAbstract
{
    public:
                        RDSyntaxNodeCameraUp();
        virtual        ~RDSyntaxNodeCameraUp();

        inline virtual void accept(RDSyntaxVisitor *visitor) override
            { visitor->visit_SyntaxNodeCameraUp(this); }

        vec3 coordinates;

};

class RDSyntaxNodeCameraFOV : public RDSyntaxNodeAbstract
{
    public:
                        RDSyntaxNodeCameraFOV();
        virtual        ~RDSyntaxNodeCameraFOV();

        inline virtual void accept(RDSyntaxVisitor *visitor) override
            { visitor->visit_SyntaxNodeCameraFOV(this); }

        r32 field_of_view;

};

class RDSyntaxNodeClipping : public RDSyntaxNodeAbstract
{

    public:
                        RDSyntaxNodeClipping();
        virtual        ~RDSyntaxNodeClipping();

        inline virtual void accept(RDSyntaxVisitor *visitor) override
            { visitor->visit_SyntaxNodeClipping(this); }

        r32 near;
        r32 far;

};

class RDSyntaxNodePoint : public RDSyntaxNodeAbstract
{
    public:
                        RDSyntaxNodePoint();
        virtual        ~RDSyntaxNodePoint();

        inline virtual void accept(RDSyntaxVisitor *visitor) override
            { visitor->visit_SyntaxNodePoint(this); }

        vec3 coordinates;
        r32 radius;

};

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
// The following context free grammar is the reference implementation of the language.
// If you're not familiar with CFGs, the basics are pretty simple. Match terms down,
// and recursively match terms based on the input. Things captured in strings are
// identifiers/keywords. Pure upper-case are token types that correspond to their
// names. For example, NUMBER is a pure integer, REAL is either pure integer or decimal.
// Since the language itself is simple to construct, there isn't much going on except
// ensure things conform to the grammar.
//
// Context Free Grammar:
//
//      root                : display frame* EOF
//
//      display             : "Display" STRING NUMBER NUMBER NUMBER
//
//      frame               : "FrameBegin" NUMBER? frame_parameters world "FrameEnd"
//
//      frame_parameters    : ("CameraAt" REAL REAL REAL)* | ("CameraEye" REAL REAL REAL)* |
//                          : ("CameraUp" REAL REAL REAL)* | ("CameraFOV" REAL)* |
//                          : ("Clipping" REAL REAL)*
//
//      world               : "WorldBegin" world_parameters "WorldEnd"
//
//      world_parameters    : ("Point" REAL REAL REAL REAL)*
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

        void        visit_root(RDSyntaxVisitor *visitor);

    protected:
        void        synchronize_to(RDViewTokenType type);
        void        display_error(RDViewToken what, RDViewTokenType expected);
        void        display_conversion_error(RDViewToken what);
        bool        convert_to_i32(i32 *in, std::string reference);
        bool        convert_to_r32(r32 *in, std::string reference);

        shared_ptr<RDSyntaxNodeAbstract> match_root();
        shared_ptr<RDSyntaxNodeAbstract> match_display();
        shared_ptr<RDSyntaxNodeAbstract> match_frame();
        shared_ptr<RDSyntaxNodeAbstract> match_frame_parameters();
        shared_ptr<RDSyntaxNodeAbstract> match_camera_at();
        shared_ptr<RDSyntaxNodeAbstract> match_camera_eye();
        shared_ptr<RDSyntaxNodeAbstract> match_camera_up();
        shared_ptr<RDSyntaxNodeAbstract> match_camera_fov();
        shared_ptr<RDSyntaxNodeAbstract> match_clipping();
        shared_ptr<RDSyntaxNodeAbstract> match_world();
        shared_ptr<RDSyntaxNodeAbstract> match_world_parameters();
        shared_ptr<RDSyntaxNodeAbstract> match_point();

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
    return new_node;
}

// --- Debug Traversal ---------------------------------------------------------
//
// The following traversal basically just dumps the AST out to console. This uses
// string stream since it will make it easier to output the final results.
//

class RDSyntaxOutputVisitor : public RDSyntaxVisitor
{

    public:
                    RDSyntaxOutputVisitor();
        virtual    ~RDSyntaxOutputVisitor();

        virtual void visit_SyntaxNodeRoot(RDSyntaxNodeRoot *node) override;
        virtual void visit_SyntaxNodeDisplay(RDSyntaxNodeDisplay *node) override;
        virtual void visit_SyntaxNodeWorld(RDSyntaxNodeWorld *node) override;
        virtual void visit_SyntaxNodeFrame(RDSyntaxNodeFrame *node) override;
        virtual void visit_SyntaxNodeCameraEye(RDSyntaxNodeCameraEye *node) override;
        virtual void visit_SyntaxNodeCameraAt(RDSyntaxNodeCameraAt *node) override;
        virtual void visit_SyntaxNodeCameraUp(RDSyntaxNodeCameraUp *node) override;
        virtual void visit_SyntaxNodeCameraFOV(RDSyntaxNodeCameraFOV *node) override;
        virtual void visit_SyntaxNodeClipping(RDSyntaxNodeClipping *node) override;
        virtual void visit_SyntaxNodePoint(RDSyntaxNodePoint *node) override;

    protected:
        u32 tab_offset;
        i32 tab_size;

        std::stringstream output_stream;

};

#endif
