#ifndef MAGNITUDE_UTILITIES_CLI_HPP
#define MAGNITUDE_UTILITIES_CLI_HPP
#include <string>
#include <vector>
#include <definitions.hpp>

class CLI;

// --- CLIArgument ---------------------------------------------------------
//
// Serves as the virtual interface for all CLI argument types.
//

enum class CLIArgumentType
{
    Error,
    Switch,
    Parameter,
    Real,
    Numeric,
    String,
};

class CLIArgument
{
    public:
                            CLIArgument(i32 index, ccptr argument);
                   virtual ~CLIArgument();

        i32                 get_index() const;
        ccptr               get_argument() const;
        CLIArgumentType     get_type() const;
        void                set_type(CLIArgumentType type) { this->type = type; };

    protected:
        CLIArgumentType type    = CLIArgumentType::Error;
        i32     raw_index       = 0;
        ccptr   raw_argument    = nullptr;

};

// --- CLIValue : CLIArgument ----------------------------------------------
//
// Arguments that don't evaluate to switches or parameters are classified
// as values. This is catch-all format (that includes error values).
//

class CLIValue : public CLIArgument
{

    public:
        static CLIArgument* parse(i32 index, ccptr argument);
        static CLIArgument* error(i32 index, ccptr argument);

        inline ccptr        get_string() const { return this->parsed_value.string; };
        inline r64          get_real() const { return this->parsed_value.real; };
        inline i64          get_signed() const { return this->parsed_value.sint; };
        inline u64          get_unsigned() const { return this->parsed_value.uint; };

    protected:
                    CLIValue(i32 argc, ccptr argument);
        virtual    ~CLIValue();

        union
        {
            ccptr   string;
            r64     real;
            i64     sint;
            u64     uint;
        } parsed_value;

};

// --- CLIParameter : CLIArgument ------------------------------------------
//
// Parameters corresponds to the "--[name]" specification.
//

class CLIParameter : public CLIArgument
{

    public:
        static CLIArgument* parse(i32 index, ccptr argument);
        std::string get_name() const;

    protected:
                        CLIParameter(i32 argc, ccptr argument);
        virtual        ~CLIParameter();

    protected:
        std::string name;

};

// --- CLISwitch : CLIArgument ---------------------------------------------
//
// Switches corresponds to the "-[A-Za-z]*" specification.
//

class CLISwitch : public CLIArgument
{

    public:
        static CLIArgument* parse(i32 index, ccptr argument);

    public:
        bool& operator[](char c);
        bool& operator[](i32 idx);

    protected:
                        CLISwitch(i32 argc, ccptr argument);
        virtual        ~CLISwitch();
        i32             offset_at(char c);

    protected:
        bool flags[52];

};

// --- CLI -----------------------------------------------------------------
//
// The global container for CLI and the parser implementation. This class
// contains only a single static implementation function called CLI::parse()
// which is responsible for classifying arguments and conforming it to the CFG.
//

class CLI
{

    public:
        static bool     parse(i32 argc, cptr* argv);
        static bool     has_flag(char c);
        static bool     has_parameter(ccptr parameter);
        static inline CLIArgument*  get(i32 i) { return self().arguments[i]; };
        static inline size_t        size() { return self().arguments.size(); };

        static void     header();
        static void     short_help();
        static void     long_help();

    protected:
                        CLI();
        virtual        ~CLI();
        static CLI&     self();

        static CLIArgument* classify();
        static CLIArgument* classify_switch();
        static CLIArgument* classify_parameter();
        static CLIArgument* classify_value();

    protected:
        std::vector<CLIArgument*> arguments;
        i32     argi;
        i32     argc;
        cptr   *argv;
        
};


#endif
