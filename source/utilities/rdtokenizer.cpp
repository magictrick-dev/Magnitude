#include <unordered_map>
#include <utilities/rdtokenizer.hpp>

inline static RDViewTokenType
string_to_type(std::string ref)
{

    // Default.
    RDViewTokenType type = RDViewTokenType::TypeIdentifier;

    // Thank you, C++, very cool!
    static std::unordered_map<std::string, RDViewTokenType> map;
    static bool map_initialized = false;
    if (map_initialized == false)
    {

        map["Display"]          = RDViewTokenType::TypeKeyDisplay;
        map["CameraAt"]         = RDViewTokenType::TypeKeyCameraAt;
        map["CameraEye"]        = RDViewTokenType::TypeKeyCameraEye;
        map["CameraUp"]         = RDViewTokenType::TypeKeyCameraUp;
        map["FrameBegin"]       = RDViewTokenType::TypeKeyFrameBegin;
        map["FrameEnd"]         = RDViewTokenType::TypeKeyFrameEnd;
        map["WorldBegin"]       = RDViewTokenType::TypeKeyWorldBegin;
        map["WorldEnd"]         = RDViewTokenType::TypeKeyWorldEnd;
        map["Point"]            = RDViewTokenType::TypeKeyPoint;
        map["Background"]       = RDViewTokenType::TypeKeyBackground;
        map["Color"]            = RDViewTokenType::TypeKeyColor;
        map["Line"]             = RDViewTokenType::TypeKeyLine;
        map["Format"]           = RDViewTokenType::TypeKeyFormat;
        map["Circle"]           = RDViewTokenType::TypeKeyCircle;
        map["Fill"]             = RDViewTokenType::TypeKeyFill;
        map["Cube"]             = RDViewTokenType::TypeKeyCube;
        map["Scale"]            = RDViewTokenType::TypeKeyScale;
        map["Translate"]        = RDViewTokenType::TypeKeyTranslate;
        map["Rotate"]           = RDViewTokenType::TypeKeyRotate;
        map["Sphere"]           = RDViewTokenType::TypeKeySphere;
        map["PolySet"]          = RDViewTokenType::TypeKeyPolySet;
        map["XformPush"]        = RDViewTokenType::TypeKeyXformPush;
        map["XformPop"]         = RDViewTokenType::TypeKeyXformPop;
        map["ObjectBegin"]      = RDViewTokenType::TypeKeyObjectBegin;
        map["ObjectEnd"]        = RDViewTokenType::TypeKeyObjectEnd;

        map["true"]             = RDViewTokenType::TypeBooleanTrue;
        map["on"]               = RDViewTokenType::TypeBooleanTrue;
        map["yes"]              = RDViewTokenType::TypeBooleanTrue;
        map["false"]            = RDViewTokenType::TypeBooleanFalse;
        map["off"]              = RDViewTokenType::TypeBooleanFalse;
        map["no"]               = RDViewTokenType::TypeBooleanFalse;

        map_initialized = true;

    }

    auto result = map.find(ref);
    if (result != map.end())
    {
        type = result->second;
    }

    return type;

}

// --- RDView Tokenizer --------------------------------------------------------
//
// I've written so many tokenizers at this point, it's actually kinda gratuitous.
//

RDViewTokenizer::
RDViewTokenizer()
{

    this->source    = "";
    this->offset    = 0;
    this->step      = 0;
    this->column    = 1;
    this->row       = 1;

    for (i32 i = 0; i < 3; ++i)
    {

        this->tokens[i].type        = RDViewTokenType::TypeEOF;
        this->tokens[i].reference   = "";
        this->tokens[i].row         = 0;
        this->tokens[i].column      = 0;

    }

    this->previous_token    = &this->tokens[0];
    this->current_token     = &this->tokens[1];
    this->next_token        = &this->tokens[2];

    // Prime that shit.
    this->shift();
    this->shift();

}

RDViewTokenizer::
RDViewTokenizer(Filepath path)
{

    rhandle res = ResourceManager::create_file_resource(path);
    MAG_ASSERT(ResourceManager::resource_handle_is_valid(res) &&
            "You should check if the file path is valid before trying to tokenizer.");
    if (!ResourceManager::resource_is_loaded(res))
        ResourceManager::load_resource(res);

    this->source    = ResourceManager::get_resource_as_string(res);
    this->offset    = 0;
    this->step      = 0;
    this->column    = 1;
    this->row       = 1;

    for (i32 i = 0; i < 3; ++i)
    {

        this->tokens[i].type        = RDViewTokenType::TypeEOF;
        this->tokens[i].reference   = "";
        this->tokens[i].row         = 0;
        this->tokens[i].column      = 0;

    }

    this->previous_token    = &this->tokens[0];
    this->current_token     = &this->tokens[1];
    this->next_token        = &this->tokens[2];

    // Prime that shit.
    this->shift();
    this->shift();

}

RDViewTokenizer::
RDViewTokenizer(std::string memory_resource)
{

    this->source    = memory_resource;
    this->offset    = 0;
    this->step      = 0;
    this->column    = 1;
    this->row       = 1;

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

void RDViewTokenizer::
reset(Filepath path)
{

    rhandle res = ResourceManager::create_file_resource(path);
    MAG_ASSERT(ResourceManager::resource_handle_is_valid(res) &&
            "You should check if the file path is valid before trying to tokenizer.");
    if (!ResourceManager::resource_is_loaded(res))
        ResourceManager::load_resource(res);

    this->source    = ResourceManager::get_resource_as_string(res);
    this->offset    = 0;
    this->step      = 0;
    this->column    = 1;
    this->row       = 1;

    for (i32 i = 0; i < 3; ++i)
    {

        this->tokens[i].type        = RDViewTokenType::TypeEOF;
        this->tokens[i].reference   = "";
        this->tokens[i].row         = 0;
        this->tokens[i].column      = 0;

    }

    this->previous_token    = &this->tokens[0];
    this->current_token     = &this->tokens[1];
    this->next_token        = &this->tokens[2];

    // Prime that shit.
    this->shift();
    this->shift();

}

void RDViewTokenizer::
reset(std::string memory_resource)
{

    this->source    = memory_resource;
    this->offset    = 0;
    this->step      = 0;
    this->column    = 1;
    this->row       = 1;

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

        if ((current == '-' || current == '+') && !isdigit(this->peek(1)))
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
            current = this->peek(0);

        }

        RDViewTokenType select = RDViewTokenType::TypeInteger;
        if (is_real == true) select = RDViewTokenType::TypeReal;
        this->next_token->type   = select;
        this->next_token->row    = this->row;
        this->next_token->column = this->column;
        this->next_token->reference.clear();
        for (i32 i = this->offset; i < this->step; ++i)
            this->next_token->reference += this->source[i];
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

        this->consume(1);
        this->synchronize();
        while (this->peek(0) != selected && !this->eof() && this->source[this->step] != '\n')
            this->consume(1);

        if (this->eof())
        {
            this->next_token->type   = RDViewTokenType::TypeError;
            this->next_token->row    = this->row;
            this->next_token->column = this->column;
            this->next_token->reference.clear();
            for (i32 i = this->offset; i < this->step; ++i)
                this->next_token->reference += this->source[i];
        }

        else if (this->source[this->step] == '\n')
        {
            this->next_token->type   = RDViewTokenType::TypeError;
            this->next_token->row    = this->row;
            this->next_token->column = this->column;
            this->next_token->reference.clear();
            for (i32 i = this->offset; i < this->step; ++i)
                this->next_token->reference += this->source[i];
        }

        else
        {

            this->next_token->type   = RDViewTokenType::TypeString;
            this->next_token->row    = this->row;
            this->next_token->column = this->column;
            this->next_token->reference.clear();
            for (i32 i = this->offset; i < this->step; ++i)
                this->next_token->reference += this->source[i];
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

        this->next_token->row    = this->row;
        this->next_token->column = this->column;
        this->next_token->reference.clear();
        for (i32 i = this->offset; i < this->step; ++i)
            this->next_token->reference += this->source[i];
        this->next_token->type = string_to_type(this->next_token->reference);
        this->synchronize();
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

    if (this->eof())
    {

        this->next_token->type          = RDViewTokenType::TypeEOF;
        this->next_token->row           = this->row;
        this->next_token->column        = this->column;
        this->next_token->reference     = "";
        return;

    }

    // The things that matter.
    if      (this->match_numbers()) return;
    else if (this->match_strings()) return;
    else if (this->match_identifiers()) return;

    // Error token.
    this->consume(1);
    this->next_token->type   = RDViewTokenType::TypeError;
    this->next_token->row    = this->row;
    this->next_token->column = this->column;
    this->next_token->reference.clear();
    for (i32 i = this->offset; i < this->step; ++i)
        this->next_token->reference += this->source[i];
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
