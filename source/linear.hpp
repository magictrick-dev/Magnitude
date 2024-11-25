#ifndef MAGNITUDE_LINEAR_HPP
#define MAGNITUDE_LINEAR_HPP
#include <definitions.hpp>

// --- Linear Algebra Library --------------------------------------------------
//
// This is *not* a fast library and it is used in place of external libraries
// for the sole purpose of it being an exercise of control on how the algorithms
// work and function. Therefore, there are instances where other libraries are
// preferred; however until then we roll our own matrix library.
//

typedef union vector2
{

    r32 elements[2];

    struct
    {
        r32 x;
        r32 y;
    };

    struct
    {
        r32 u;
        r32 v;
    };

    struct
    {
        r32 width;
        r32 height;
    };

} vec2, v2;

typedef union vector3
{
    
    r32 elements[3];

    struct
    {
        r32 x;
        r32 y;
        r32 z;
    };

    struct
    {
        r32 r;
        r32 g;
        r32 b;
    };

    struct
    {
        r32 width;
        r32 height;
        r32 depth;
    };

} vec3, v3;

typedef union vector4
{

    r32 elements[4];

    struct
    {
        r32 x;
        r32 y;
        r32 z;
        r32 w;
    };

    struct
    {
        r32 r;
        r32 g;
        r32 b;
        r32 a;
    };

    struct
    {
        r32 s;
        r32 t;
        r32 u;
        r32 v;
    };

} vec4, v4, homogenous_point, hp;

#endif
