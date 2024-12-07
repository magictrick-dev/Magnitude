#ifndef MAGNITUDE_ENVIRONMENT_HPP
#define MAGNITUDE_ENVIRONMENT_HPP
#include <common.hpp>
#include <bindings.hpp>
#include <rdparser/rdparser.hpp>
#include <renderer/worldsim.hpp>
#include <renderer/framebuffer.hpp>

class Environment
{
    
    public:
        static Environment& get();

    public:
        Framebuffer         framebuffer;
        RDSyntaxParser      parser;
        Worldsim            world;

    protected:
                        Environment();

};

#endif
