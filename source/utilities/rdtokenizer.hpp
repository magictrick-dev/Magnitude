#ifndef MAGNITUDE_UTILITIES_RDTOKENIZER_HPP
#define MAGNITUDE_UTILITIES_RDTOKENIZER_HPP
#include <string>
#include <definitions.hpp>
#include <utilities/path.hpp>
#include <utilities/resourceman.hpp>

// --- RDView Tokenizer --------------------------------------------------------
//
// This tokenizes RDView source files. If you've ever written a parser before,
// this would probably look *very* familiar to you. Since RDView script files
// are pretty simple, the parser only really needs to validate identifiers,
// strings, and real numbers. Keywords are case-insensitive, whitespace isn't
// important, and comments begin with "#". The language specification is outlined
// in the parser if you care to learn more.
//

enum class RDViewTokenType
{
    TypeError       =  -2,
    TypeEOF         =  -1,
    TypeReal        =   0, // Basic float formats, can prceed +/-.
    TypeInteger     =   1, // Basic integer formats, can preceed +/-.
    TypeString      =   2, // Conventionally double, but we can do singles.
    TypeBoolean     =   3, // true, yes, on, or 1 : false, no, off, or 0
    TypeIdentifier  =   4, // Anything not classified as a direct keyword.
};

struct RDViewToken
{

    RDViewTokenType     type;
    std::string         reference;
    i32                 row;
    i32                 column;

};

class RDViewTokenizer
{

    public:
                            RDViewTokenizer(Filepath path);
                            RDViewTokenizer(std::string memory_resource);
        virtual            ~RDViewTokenizer();

        void                shift();
        RDViewToken         get_previous() const;
        RDViewToken         get_current() const;
        RDViewToken         get_next() const;

    protected:
        bool                consume_whitespace();
        
        bool                match_numbers();
        bool                match_strings();
        bool                match_identifiers();

        bool                eof() const;
        char                peek(i32 how_far) const;
        void                consume(i32 how_many);
        void                synchronize();

    protected:
        std::string         source;

        RDViewToken         tokens[3];
        RDViewToken*        previous_token;
        RDViewToken*        current_token;
        RDViewToken*        next_token;

        i32                 step;
        i32                 offset;
        i32                 row;
        i32                 column;

};

#endif
