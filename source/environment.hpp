#ifndef MAGNITUDE_ENVIRONMENT_HPP
#define MAGNITUDE_ENVIRONMENT_HPP
#include <common.hpp>
#include <bindings.hpp>
#include <rdparser/rdparser.hpp>
#include <renderer/worldsim.hpp>

class Environment
{
    
    public:
        static Environment& get();

    public:
        RDSyntaxParser      parser;
        Worldsim            world;

    protected:
                        Environment();

};

#endif
