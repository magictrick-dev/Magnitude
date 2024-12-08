#ifndef MAGNITUDE_LINEAR_HPP
#define MAGNITUDE_LINEAR_HPP
#include <definitions.hpp>
#include <iostream>
#include <string>

// TODO(Chris): We will need to macro-ify this since not all platforms support
//              the use of SSE (basically ARM processors which use NEON).
//
//              Alternatively, we can just build a typedef library that basically
//              masks these operations out into intrinsic wrappers. That would be
//              really cute and helpful.
#include <xmmintrin.h>

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

vector2 operator-(const vector2& lhs);
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
    r32         magnitude() const;
    vector3     normalize() const;

    r32&        operator[](i32 idx);
    r32         operator[](i32 idx) const;

} vec3, v3;

vector3 cross_product(const vector3& lhs, const vector3& rhs);
r32 dot_product(const vector3& lhs, const vector3& rhs);

vector3 operator-(const vector3& lhs);
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

        r32     elements[4]; 
        __m128  wide;

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

r32 dot_product(const vector4& lhs, const vector4& rhs);

vector4 operator-(const vector4& lhs);
vector4 operator+(const vector4& lhs, const vector4& rhs);
vector4 operator-(const vector4& lhs, const vector4& rhs);
vector4 operator*(const vector4& lhs, const r32& rhs);
vector4 operator*(const r32& lhs, const vector4& rhs);
vector4 operator/(const vector4& lhs, const r32& rhs);
vector4 operator/(const r32& lhs, const vector4& rhs);
bool    operator==(const vector4& lhs, const vector4& rhs);

std::ostream& operator<<(std::ostream& os, const vector4& rhs);
std::string to_string(const vector4& rhs);

typedef struct matrix4
{

    union
    {

        r32     elements[4][4];
        vector4 columns[4];

    };

    vector4&    operator[](i32 idx);
    vector4     operator[](i32 idx) const;
    r32&        at(i32 row, i32 col);
    r32         at(i32 row, i32 col) const;

} mat4, m4;

matrix4 matrix4_identity();
matrix4 matrix4_diagonal(r32 d);
matrix4 matrix4_translate(vector3 p);
matrix4 matrix4_scale(vector3 s);
matrix4 matrix4_perspective_rh(r32 fov, r32 aspect_ratio, r32 near, r32 far);
matrix4 matrix4_camera_lookat(vec3 eye, vec3 at, vec3 up);

matrix4 operator*(const matrix4& left, const r32& right);
matrix4 operator/(const matrix4& left, const r32& right);

vector4 operator*(const matrix4& left, const vector4& right);
matrix4 operator*(const matrix4& left, const matrix4& right);

std::ostream& operator<<(std::ostream& os, const matrix4& rhs);
std::string to_string(const matrix4& rhs);

#endif
