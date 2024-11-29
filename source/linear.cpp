#include <linear.hpp>

// --- Vector 2D ---------------------------------------------------------------
//
// Implementation of vector 2D operations.
//

r32& vector2::
operator[](i32 idx)
{
    return this->elements[idx];
}

r32 vector2::
operator[](i32 idx) const
{
    return this->elements[idx];
}

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

bool
operator==(const vector2& lhs, const vector2& rhs)
{

    bool result = (lhs[0] == rhs[0] &&
                   lhs[1] == rhs[1]);
    return result;

}

std::ostream& 
operator<<(std::ostream& os, const vector2& rhs)
{

    os << rhs.x << " " << rhs.y;
    return os;

}

std::string 
to_string(const vector2& rhs)
{

    std::string result;
    result += rhs.x;
    result += " ";
    result += rhs.y;
    return result;

}

// --- Vector 3D ---------------------------------------------------------------
//
// Implementation of vector 3D operations.
//

r32& vector3::
operator[](i32 idx)
{
    return this->elements[idx];
}

r32 vector3::
operator[](i32 idx) const
{
    return this->elements[idx];
}

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

bool
operator==(const vector3& lhs, const vector3& rhs)
{

    bool result = (lhs[0] == rhs[0] &&
                   lhs[1] == rhs[1] &&
                   lhs[2] == rhs[2]);
    return result;

}

std::ostream& 
operator<<(std::ostream& os, const vector3& rhs)
{

    os << rhs.x << " " << rhs.y << " " << rhs.z;
    return os;

}

std::string 
to_string(const vector3& rhs)
{

    std::string result;
    result += rhs.x;
    result += " ";
    result += rhs.y;
    result += " ";
    result += rhs.z;
    return result;

}

// --- Vector 3D Homogenous ----------------------------------------------------
//
// The bread and butter of computer graphics: three-dimensional homogenous vectors.
//

r32& vector4::
operator[](i32 idx)
{
    return this->elements[idx];
}

r32 vector4::
operator[](i32 idx) const
{
    return this->elements[idx];
}

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

bool
operator==(const vector4& lhs, const vector4& rhs)
{

    bool result = (lhs[0] == rhs[0] &&
                   lhs[1] == rhs[1] &&
                   lhs[2] == rhs[2] &&
                   lhs[3] == rhs[3]);
    return result;

}

r32 vector4::
magnitude_squared() const
{

    r32 result = this->elements[0] * this->elements[0] +
                 this->elements[1] * this->elements[1] +
                 this->elements[2] * this->elements[2];
    return result;

}

r32 vector4::
magnitude() const
{

    r32 result = this->elements[0] * this->elements[0] +
                 this->elements[1] * this->elements[1] +
                 this->elements[2] * this->elements[2];
    return sqrtf(result);

}

vector4 vector4::
normalize() const
{

    r32 magnitude = this->magnitude();
    vector4 result = *this;
    result /= magnitude;
    return result;

}

vector4 vector4::
homogenize() const
{

    vector4 result = *this;
    result /= result.w;
    result.w = 1.0f;
    return result;

}

std::ostream& 
operator<<(std::ostream& os, const vector4& rhs)
{

    os << rhs.x << " " << rhs.y << " " << rhs.z << " " << rhs.w;
    return os;

}

std::string 
to_string(const vector4& rhs)
{

    std::string result;
    result += rhs.x;
    result += " ";
    result += rhs.y;
    result += " ";
    result += rhs.z;
    result += " ";
    result += rhs.w;
    return result;

}
