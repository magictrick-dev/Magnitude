#include <utilities/rdtokenizer.hpp>

// --- RDView Tokenizer --------------------------------------------------------
//
// I've written so many tokenizers at this point, it's actually kinda gratuitous.
//

RDViewTokenizer::
RDViewTokenizer(Filepath path)
{

    rhandle res = ResourceManager::create_file_resource(path);
    MAG_ASSERT(ResourceManager::resource_handle_is_valid(res) &&
            "You should check if the file path is valid before trying to tokenizer.");
    if (!ResourceManager::resource_is_loaded(res))
        ResourceManager::load_resource(res);

    this->source = ResourceManager::get_resource_as_string(res);

    for (i32 i = 0; i < 3; ++i)
    {

        this->tokens[i].type        = RDViewTokenType::TypeEOF;
        this->tokens[i].reference   = "";
        this->tokens[i].row         = 0;
        this->tokens[i].column      = 0;

    }

    // Prime that shit.
    this->shift();
    this->shift();

}

RDViewTokenizer::
RDViewTokenizer(std::string memory_resource)
{

    this->source = memory_resource;

    for (i32 i = 0; i < 3; ++i)
    {

        this->tokens[i].type        = RDViewTokenType::TypeEOF;
        this->tokens[i].reference   = "";
        this->tokens[i].row         = 0;
        this->tokens[i].column      = 0;

    }

    // Prime that shit.
    this->shift();
    this->shift();

}

RDViewTokenizer::
~RDViewTokenizer()
{



}

bool RDViewTokenizer::
eof() const
{

    if (this->source[this->step] == '\0')
    {

        return true;

    }


    return false;

}

char RDViewTokenizer::
peek(i32 h) const
{

    // TODO(Chris): This could potentially go out of bounds...
    //              The smart thing would be to check, but I ain't gettin' paid
    //              enough for that, so here we are.
    char result = this->source[this->step + h];
    return result;

}

void RDViewTokenizer::
consume(i32 h)
{

    for (i32 i = 0; i < h; ++i)
    {

        if (this->source[this->step] == '\0') 
            break;

        if (this->source[this->step] == '\n')
        {
            this->row++;
            this->column = 0;
        }

        this->column++;
        this->step++;

    }

}

void RDViewTokenizer::
synchronize()
{

    this->offset = this->step;

}

bool RDViewTokenizer::
consume_whitespace()
{

    // All the important whitespaces... Edge-cases can go kick rocks.
    if (this->source[this->step] == ' '     ||
        this->source[this->step] == '\r'    ||
        this->source[this->step] == '\n'    ||
        this->source[this->step] == '\t')
    {

        this->consume(1);
        this->synchronize();
        return true;

    }

    // Comments are nuked from orbit.
    else if (this->source[this->step] == '#')
    {

        while (!this->eof() && this->source[this->step] != '\n')
        {

            this->consume(1);

        }

        this->consume(1);
        this->synchronize();
        return true;

    }

    return false;

}

bool RDViewTokenizer::
match_numbers()
{

    char current = this->peek(0);
    if (isdigit(current) || current == '-' || current == '+')
    {

        if (current == '-' || current == '+' && !isdigit(this->peek(1)))
            return false;
        this->consume(1);

        char current = this->peek(0);
        bool is_real = false;
        while (!this->eof())
        {

            if (current == '.' && is_real == false)
            {
                is_real = true;
            }
            else if (current == '.' && is_real == true)
            {
                break;
            }
            else if (!isdigit(current))
            {
                break;
            }

            this->consume(1);

        }

        RDViewTokenType select = RDViewTokenType::TypeInteger;
        if (is_real == true) select = RDViewTokenType::TypeReal;
        this->current_token->type   = select;
        this->current_token->row    = this->row;
        this->current_token->column = this->column;
        for (i32 i = this->offset; i < this->step; ++i)
            this->current_token->reference += this->source[i];
        this->synchronize();
        return true;

    }

    return false;

}

bool RDViewTokenizer::
match_strings()
{

    char current = this->peek(0);

    char selected = current;
    if (current == '"' || current == '\'')
    {

        while (this->peek(0) != selected && !this->eof() && this->source[this->step] != '\n')
            this->consume(1);

        if (this->eof())
        {
            this->current_token->type   = RDViewTokenType::TypeError;
            this->current_token->row    = this->row;
            this->current_token->column = this->column;
            for (i32 i = this->offset; i < this->step; ++i)
                this->current_token->reference += this->source[i];
        }

        else if (this->source[this->step] == '\n')
        {
            this->current_token->type   = RDViewTokenType::TypeError;
            this->current_token->row    = this->row;
            this->current_token->column = this->column;
            for (i32 i = this->offset; i < this->step; ++i)
                this->current_token->reference += this->source[i];
        }

        else
        {

            this->current_token->type   = RDViewTokenType::TypeString;
            this->current_token->row    = this->row;
            this->current_token->column = this->column;
            for (i32 i = this->offset; i < this->step; ++i)
                this->current_token->reference += this->source[i];
            this->consume(1);

        }

        this->synchronize();
        return true;

    }

    return false;

}

bool RDViewTokenizer::
match_identifiers()
{

    char current = this->peek(0);
    if (isalpha(current))
    {

        this->consume(1);
        current = this->peek(0);

        while (isalnum(current) || current == '_')
        {
            this->consume(1);
            current = this->peek(0);
        }

        this->current_token->type   = RDViewTokenType::TypeIdentifier;
        this->current_token->row    = this->row;
        this->current_token->column = this->column;
        for (i32 i = this->offset; i < this->step; ++i)
            this->current_token->reference += this->source[i];
        this->synchronize();

        // TODO(Chris): Not all identifiers are created equal, some are keywords.
        //              Keyword checking is on the list of shit to do.

        return true;

    }

    return false;

}

void RDViewTokenizer::
shift()
{

    // Swap some pointers.
    RDViewToken *temporary      = this->previous_token;
    this->previous_token        = this->current_token;
    this->current_token         = this->next_token;
    this->next_token            = temporary;

    // Yeet the whitespace & comments.
    while (this->consume_whitespace());

    // The things that matter.
    if      (this->match_numbers()) return;
    else if (this->match_strings()) return;
    else if (this->match_identifiers()) return;

    // Error token.
    this->consume(1);
    this->current_token->type   = RDViewTokenType::TypeError;
    this->current_token->row    = this->row;
    this->current_token->column = this->column;
    for (i32 i = this->offset; i < this->step; ++i)
        this->current_token->reference += this->source[i];
    this->synchronize();

    return;

}

RDViewToken RDViewTokenizer::
get_previous() const
{

    return *this->previous_token;

}

RDViewToken RDViewTokenizer::
get_current() const
{

    return *this->current_token;

}

RDViewToken RDViewTokenizer::
get_next() const
{

    return *this->next_token;

}
