#ifndef MAGNITUDE_RENDERER_WORLDSIM_HPP
#define MAGNITUDE_RENDERER_WORLDSIM_HPP
#include <common.hpp>
#include <vector>
#include <string>
#include <renderer/pipeline.hpp>

// --- Entity Interface --------------------------------------------------------
//
// There are a limitted set of entities that can be rendered, but generally have
// very different render pipelines. The basic interface essentially describes the
// entity, which it can then be rendered dynamically, as needed.
//

class IEntity
{

    public:
        inline          IEntity() {}
        inline virtual ~IEntity() {}

        inline std::string  get_name() const { return this->name; }
        inline i32          get_id() const { return this->id; }

        inline virtual void submit() = 0;

    protected:
        std::string name = "";
        i32 id = -1;

};

// --- Point Entity ------------------------------------------------------------
//
// The point entity represents basic points.
//

class PointEntity : public IEntity
{

    public:
        inline          PointEntity(i32 id, vec3 pos, r32 radius);
        inline         ~PointEntity();

        inline void     submit() override;

    protected:
        vec3 position;
        r32 radius;

};

PointEntity::
PointEntity(i32 id, vec3 pos, r32 radius)
{

    this->id = id;
    this->name = "Point";
    this->position = pos;
    this->radius = radius;

}

PointEntity::
~PointEntity()
{

}

void PointEntity::
submit()
{

    RendererPipeline::point_pipeline(this->position, this->radius);

}

// --- World Simulation --------------------------------------------------------
//
// The world simulation contains the set of all entities to be rendered.
// Since we want to preserve some information about the state of the simulation,
// such as free-camera vs. static-camera positions, this is a very simple way to
// do so. As a frame is selected, the list of entities placed within are updated.
//

class Worldsim
{

    public:
        template <class T, class... Args> inline i32 create_entity(Args... args);

        inline void clear_entities();

    protected:
        i32 id_stack;
        std::vector<shared_ptr<IEntity>> entity_list;

};

template <class T, class... Args> i32 Worldsim::
create_entity(Args... args)
{

    shared_ptr<T> new_entity = std::make_shared<T>(this->id_stack++, &args...);
    shared_ptr<IEntity> pushable = dynamic_pointer_cast<IEntity>(new_entity);
    this->entity_list.push_back(pushable);

}

void Worldsim::
clear_entities()
{

    entity_list.clear();
    id_stack = 0;

}

#endif
