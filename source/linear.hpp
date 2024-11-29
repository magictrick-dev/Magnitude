#ifndef MAGNITUDE_LINEAR_HPP
#define MAGNITUDE_LINEAR_HPP
#include <definitions.hpp>
#include <iostream>
#include <string>

// --- Linear Algebra Library --------------------------------------------------
//
// This is *not* a fast library and it is used in place of external libraries
// for the sole purpose of it being an exercise of control on how the algorithms
// work and function. Therefore, there are instances where other libraries are
// preferred; however until then we roll our own matrix library.
//

typedef struct vector2
{

    union
    {

        r32 elements[2] = { 0.0f, 0.0f };

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

    };

    vector2&    operator+=(const vector2& rhs);
    vector2&    operator-=(const vector2& rhs);
    vector2&    operator*=(const r32& rhs);
    vector2&    operator/=(const r32& rhs);

    r32         magnitude_squared() const;
    r32         magntidue() const;
    vector2     normalize() const;

    r32&        operator[](i32 idx);
    r32         operator[](i32 idx) const;

} vec2, v2;

vector2 operator+(const vector2& lhs, const vector2& rhs);
vector2 operator-(const vector2& lhs, const vector2& rhs);
vector2 operator*(const vector2& lhs, const r32& rhs);
vector2 operator*(const r32& lhs, const vector2& rhs);
vector2 operator/(const vector2& lhs, const r32& rhs);
vector2 operator/(const r32& lhs, const vector2& rhs);
bool    operator==(const vector2& lhs, const vector2& rhs);

std::ostream& operator<<(std::ostream& os, const vector2& rhs);
std::string to_string(const vector2& rhs);

typedef struct vector3
{
    
    union
    {

        r32 elements[3] = { 0.0f, 0.0f, 0.0f };

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

    };

    vector3&    operator+=(const vector3& rhs);
    vector3&    operator-=(const vector3& rhs);
    vector3&    operator*=(const r32& rhs);
    vector3&    operator/=(const r32& rhs);

    r32         magnitude_squared() const;
    r32         magntidue() const;
    vector3     normalize() const;

    r32&        operator[](i32 idx);
    r32         operator[](i32 idx) const;

} vec3, v3;

vector3 operator+(const vector3& lhs, const vector3& rhs);
vector3 operator-(const vector3& lhs, const vector3& rhs);
vector3 operator*(const vector3& lhs, const r32& rhs);
vector3 operator*(const r32& lhs, const vector3& rhs);
vector3 operator/(const vector3& lhs, const r32& rhs);
vector3 operator/(const r32& lhs, const vector3& rhs);
bool    operator==(const vector3& lhs, const vector3& rhs);

std::ostream& operator<<(std::ostream& os, const vector3& rhs);
std::string to_string(const vector3& rhs);

typedef struct vector4
{

    union {

        r32 elements[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

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

    };

    vector4&    operator+=(const vector4& rhs);
    vector4&    operator-=(const vector4& rhs);
    vector4&    operator*=(const r32& rhs);
    vector4&    operator/=(const r32& rhs);

    r32&        operator[](i32 idx);
    r32         operator[](i32 idx) const;

    r32         magnitude_squared() const;
    r32         magnitude() const;
    vector4     normalize() const;
    vector4     homogenize() const;

} vec4, v4, homogenous_point, hp;

vector4 operator+(const vector4& lhs, const vector4& rhs);
vector4 operator-(const vector4& lhs, const vector4& rhs);
vector4 operator*(const vector4& lhs, const r32& rhs);
vector4 operator*(const r32& lhs, const vector4& rhs);
vector4 operator/(const vector4& lhs, const r32& rhs);
vector4 operator/(const r32& lhs, const vector4& rhs);
bool    operator==(const vector4& lhs, const vector4& rhs);

std::ostream& operator<<(std::ostream& os, const vector4& rhs);
std::string to_string(const vector4& rhs);

#endif
