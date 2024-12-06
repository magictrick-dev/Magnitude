#ifndef MAGNITUDE_RENDERER_WORLDSIM_HPP
#define MAGNITUDE_RENDERER_WORLDSIM_HPP
#include <common.hpp>
#include <vector>
#include <string>

class Entity
{

    public:
        inline          Entity() {}
        inline virtual ~Entity() {}

        inline std::string  get_name() const { return this->name; }
        inline i32          get_id() const { return this->id; }

    public:
        vec3 position = { 0.0f, 0.0f, 0.0f };
        vec3 rotation = { 0.0f, 0.0f, 0.0f };

    protected:
        std::string name = "";
        i32 id = -1;

};

class Worldsim
{

    public:
        std::vector<Entity> entity_list;

};

#endif
