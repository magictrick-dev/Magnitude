#ifndef MAGNITUDE_UTILITIES_RDTOKENIZER_HPP
#define MAGNITUDE_UTILITIES_RDTOKENIZER_HPP
#include <string>
#include <definitions.hpp>
#include <utilities/path.hpp>

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
    TypeReal        =   0,
    TypeString      =   1,
    TypeIdentifier  =   2,
};

class RDViewToken
{

    public:
                            RDViewToken();
        virtual            ~RDViewToken();

        RDViewTokenType     get_type()      const;
        i32                 get_offset()    const;
        i32                 get_length()    const;
        std::pair<i32, i32> get_location()  const;
        Filepath            get_filepath()  const;

    protected:
        RDViewTokenType     type;
        i32                 offset;
        i32                 length;

};

class RDViewTokenizer
{

    public:
                            RDViewTokenizer();
        virtual            ~RDViewTokenizer();

        void                shift();
        RDViewToken         get_previous() const;
        RDViewToken         get_current() const;
        RDViewToken         get_next() const;

        bool                is_eof();

    protected:
        Filepath            path;
        std::string         source;

        RDViewToken         tokens[3];
        RDViewToken*        previous_token;
        RDViewToken*        current_token;
        RDViewToken*        next_token;

        i32                 step;
        i32                 offset;

};

#endif
