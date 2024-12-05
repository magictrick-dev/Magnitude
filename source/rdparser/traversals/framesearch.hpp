#ifndef MAGNITUDE_RDPARSER_TRAVERSALS_FRAMESEARCH_HPP
#define MAGNITUDE_RDPARSER_TRAVERSALS_FRAMESEARCH_HPP
#include <rdparser/rdparser.hpp>

class RDSyntaxFrameSearch : public RDSyntaxVisitor
{

    public:
        inline virtual void visit_SyntaxNodeRoot(RDSyntaxNodeRoot *node) override;
        inline virtual void visit_SyntaxNodeFrame(RDSyntaxNodeFrame *node) override;

    public:
        std::vector<RDSyntaxNodeFrame*> frames;

};

inline void RDSyntaxFrameSearch::
visit_SyntaxNodeRoot(RDSyntaxNodeRoot *node)
{

    for (shared_ptr<RDSyntaxNodeAbstract> frame : node->frames)
        frame->accept(this);

}

inline void RDSyntaxFrameSearch::
visit_SyntaxNodeFrame(RDSyntaxNodeFrame *node)
{

    this->frames.push_back(node);

}

#endif
