#include <linear.hpp>

// --- Vector 2D ---------------------------------------------------------------
//
// Implementation of vector 2D operations.
//

vector2& vector2::
operator+=(const vector2& rhs)
{

    this->elements[0] += rhs.elements[0];
    this->elements[1] += rhs.elements[1];
    return *this;

}

vector2& vector2::
operator-=(const vector2& rhs)
{

    this->elements[0] -= rhs.elements[0];
    this->elements[1] -= rhs.elements[1];
    return *this;

}

vector2& vector2::
operator*=(const r32& rhs)
{

    this->elements[0] *= rhs;
    this->elements[1] *= rhs;
    return *this;

}

vector2& vector2::
operator/=(const r32& rhs)
{

    this->elements[0] /= rhs;
    this->elements[1] /= rhs;
    return *this;

}

vector2 
operator+(const vector2& lhs, const vector2& rhs)
{

    vector2 result = lhs;
    result += rhs;
    return result;

}

vector2 
operator-(const vector2& lhs, const vector2& rhs)
{

    vector2 result = lhs;
    result -= rhs;
    return result;

}

vector2 
operator*(const vector2& lhs, const r32& rhs)
{

    vector2 result = lhs;
    result *= rhs;
    return result;

}

vector2 
operator*(const r32& lhs, const vector2& rhs)
{

    vector2 result = rhs;
    result *= lhs;
    return result;

}

vector2 
operator/(const vector2& lhs, const r32& rhs)
{

    vector2 result = lhs;
    result /= rhs;
    return result;

}

vector2 
operator/(const r32& lhs, const vector2& rhs)
{

    vector2 result = rhs;
    result /= lhs;
    return result;

}

// --- Vector 3D ---------------------------------------------------------------
//
// Implementation of vector 3D operations.
//

vector3& vector3::
operator+=(const vector3& rhs)
{

    this->elements[0] += rhs.elements[0];
    this->elements[1] += rhs.elements[1];
    this->elements[2] += rhs.elements[2];
    return *this;

}

vector3& vector3::
operator-=(const vector3& rhs)
{

    this->elements[0] -= rhs.elements[0];
    this->elements[1] -= rhs.elements[1];
    this->elements[2] -= rhs.elements[2];
    return *this;

}

vector3& vector3::
operator*=(const r32& rhs)
{

    this->elements[0] *= rhs;
    this->elements[1] *= rhs;
    this->elements[2] *= rhs;
    return *this;

}

vector3& vector3::
operator/=(const r32& rhs)
{

    this->elements[0] /= rhs;
    this->elements[1] /= rhs;
    this->elements[2] /= rhs;
    return *this;

}

vector3 
operator+(const vector3& lhs, const vector3& rhs)
{

    vector3 result = lhs;
    result += rhs;
    return result;

}

vector3 
operator-(const vector3& lhs, const vector3& rhs)
{

    vector3 result = lhs;
    result -= rhs;
    return result;

}

vector3 
operator*(const vector3& lhs, const r32& rhs)
{

    vector3 result = lhs;
    result *= rhs;
    return result;

}

vector3 
operator*(const r32& lhs, const vector3& rhs)
{

    vector3 result = rhs;
    result *= lhs;
    return result;

}

vector3 
operator/(const vector3& lhs, const r32& rhs)
{

    vector3 result = lhs;
    result /= rhs;
    return result;

}

vector3 
operator/(const r32& lhs, const vector3& rhs)
{

    vector3 result = rhs;
    result /= lhs;
    return result;

}

// --- Vector 3D Homogenous ----------------------------------------------------
//
// The bread and butter of computer graphics: three-dimensional homogenous vectors.
//

vector4& vector4::
operator+=(const vector4& rhs)
{

    MAG_ASSERT(rhs.w == this->w); // Should have the same w-component... otherwise
                                  // we will need to scale back.

    this->elements[0] += rhs.elements[0];
    this->elements[1] += rhs.elements[1];
    this->elements[2] += rhs.elements[2];
    return *this;

}

vector4& vector4::
operator-=(const vector4& rhs)
{

    MAG_ASSERT(rhs.w == this->w); // Should have the same w-component... otherwise
                                  // we will need to scale back.
    
    this->elements[0] -= rhs.elements[0];
    this->elements[1] -= rhs.elements[1];
    this->elements[2] -= rhs.elements[2];
    return *this;

}

vector4& vector4::
operator*=(const r32& rhs)
{

    this->elements[0] *= rhs;
    this->elements[1] *= rhs;
    this->elements[2] *= rhs;
    return *this;

}

vector4& vector4::
operator/=(const r32& rhs)
{

    this->elements[0] /= rhs;
    this->elements[1] /= rhs;
    this->elements[2] /= rhs;
    return *this;

}

vector4 
operator+(const vector4& lhs, const vector4& rhs)
{

    vector4 result = lhs;
    result += rhs;
    return result;

}

vector4 
operator-(const vector4& lhs, const vector4& rhs)
{

    vector4 result = lhs;
    result -= rhs;
    return result;

}

vector4 
operator*(const vector4& lhs, const r32& rhs)
{

    vector4 result = lhs;
    result *= rhs;
    return result;

}

vector4 
operator*(const r32& lhs, const vector4& rhs)
{

    vector4 result = rhs;
    result *= lhs;
    return result;

}

vector4 
operator/(const vector4& lhs, const r32& rhs)
{

    vector4 result = lhs;
    result /= rhs;
    return result;

}

vector4 
operator/(const r32& lhs, const vector4& rhs)
{

    vector4 result = rhs;
    result /= lhs;
    return result;

}
