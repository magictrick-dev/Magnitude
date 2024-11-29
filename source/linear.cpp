#include <linear.hpp>

// --- Helpers -----------------------------------------------------------------

static inline vector4
linear_combine_matrix(matrix4 matrix, vector4 vector)
{

    vector4 result;

    result.x  = vector.elements[0] * matrix.columns[0].x;
    result.y  = vector.elements[0] * matrix.columns[0].y;
    result.z  = vector.elements[0] * matrix.columns[0].z;
    result.w  = vector.elements[0] * matrix.columns[0].w;

    result.x += vector.elements[1] * matrix.columns[1].x;
    result.y += vector.elements[1] * matrix.columns[1].y;
    result.z += vector.elements[1] * matrix.columns[1].z;
    result.w += vector.elements[1] * matrix.columns[1].w;

    result.x += vector.elements[2] * matrix.columns[2].x;
    result.y += vector.elements[2] * matrix.columns[2].y;
    result.z += vector.elements[2] * matrix.columns[2].z;
    result.w += vector.elements[2] * matrix.columns[2].w;

    result.x += vector.elements[3] * matrix.columns[3].x;
    result.y += vector.elements[3] * matrix.columns[3].y;
    result.z += vector.elements[3] * matrix.columns[3].z;
    result.w += vector.elements[3] * matrix.columns[3].w;

    return result;

}

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
    result += std::to_string(rhs.x);
    result += " ";
    result += std::to_string(rhs.y);
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
    result += std::to_string(rhs.x); 
    result += " "; 
    result += std::to_string(rhs.y);
    result += " ";
    result += std::to_string(rhs.z);
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
    result += std::to_string(rhs.x);
    result += " ";
    result += std::to_string(rhs.y);
    result += " ";
    result += std::to_string(rhs.z);
    result += " ";
    result += std::to_string(rhs.w);
    return result;

}

// --- Matrix 4D ---------------------------------------------------------------
//
// Pretty self-explanatory stuff here. Just a big ol' cube of numbers.
//

vector4& matrix4::
operator[](i32 idx)
{
    return this->columns[idx];
}

vector4 matrix4::
operator[](i32 idx) const
{
    return this->columns[idx];
}

r32& matrix4::
at(i32 row, i32 col)
{
    return this->elements[row][col];
}

r32 matrix4::
at(i32 row, i32 col) const
{
    return this->elements[row][col];
}

matrix4
matrix4_identity()
{

    matrix4 result  = {0};
    result[0][0]    = 1.0f;
    result[1][1]    = 1.0f;
    result[2][2]    = 1.0f;
    result[3][3]    = 1.0f;
    return result;

}

matrix4
matrix4_diagonal(r32 d)
{

    matrix4 result  = {0};
    result[0][0]    = d;
    result[1][1]    = d;
    result[2][2]    = d;
    result[3][3]    = d;
    return result;

}

matrix4 
operator*(const matrix4& left, const r32& right)
{

    matrix4 result;

    result.elements[0][0] = left.elements[0][0] * right;
    result.elements[0][1] = left.elements[0][1] * right;
    result.elements[0][2] = left.elements[0][2] * right;
    result.elements[0][3] = left.elements[0][3] * right;

    result.elements[1][0] = left.elements[1][0] * right;
    result.elements[1][1] = left.elements[1][1] * right;
    result.elements[1][2] = left.elements[1][2] * right;
    result.elements[1][3] = left.elements[1][3] * right;

    result.elements[2][0] = left.elements[2][0] * right;
    result.elements[2][1] = left.elements[2][1] * right;
    result.elements[2][2] = left.elements[2][2] * right;
    result.elements[2][3] = left.elements[2][3] * right;

    result.elements[3][0] = left.elements[3][0] * right;
    result.elements[3][1] = left.elements[3][1] * right;
    result.elements[3][2] = left.elements[3][2] * right;
    result.elements[3][3] = left.elements[3][3] * right;

    return result;

}

matrix4 
operator/(const matrix4& left, const r32& right)
{

    matrix4 result;

    result.elements[0][0] = left.elements[0][0] / right;
    result.elements[0][1] = left.elements[0][1] / right;
    result.elements[0][2] = left.elements[0][2] / right;
    result.elements[0][3] = left.elements[0][3] / right;

    result.elements[1][0] = left.elements[1][0] / right;
    result.elements[1][1] = left.elements[1][1] / right;
    result.elements[1][2] = left.elements[1][2] / right;
    result.elements[1][3] = left.elements[1][3] / right;

    result.elements[2][0] = left.elements[2][0] / right;
    result.elements[2][1] = left.elements[2][1] / right;
    result.elements[2][2] = left.elements[2][2] / right;
    result.elements[2][3] = left.elements[2][3] / right;

    result.elements[3][0] = left.elements[3][0] / right;
    result.elements[3][1] = left.elements[3][1] / right;
    result.elements[3][2] = left.elements[3][2] / right;
    result.elements[3][3] = left.elements[3][3] / right;

    return result;

}

vector4 
operator*(const matrix4& left, const vector4& right)
{

    vector4 result = linear_combine_matrix(left, right);
    return result;

}

matrix4
operator*(const matrix4& left, const matrix4& right)
{

    matrix4 result;
    result.columns[0] = linear_combine_matrix(left, right.columns[0]);
    result.columns[1] = linear_combine_matrix(left, right.columns[1]);
    result.columns[2] = linear_combine_matrix(left, right.columns[2]);
    result.columns[3] = linear_combine_matrix(left, right.columns[3]);
    return result;

}

matrix4
matrix4_translate(vector3 p)
{

    matrix4 result = matrix4_identity();
    result.elements[3][0] = p.x;
    result.elements[3][1] = p.y;
    result.elements[3][2] = p.z;
    return result;

}

matrix4
matrix4_scale(vector3 s)
{

    matrix4 result = matrix4_identity();
    result.elements[0][0] = s.x;
    result.elements[1][1] = s.y;
    result.elements[2][2] = s.z;
    return result;

}

std::ostream& 
operator<<(std::ostream& os, const matrix4& rhs)
{
    os  << rhs.columns[0] << std::endl
        << rhs.columns[1] << std::endl
        << rhs.columns[2] << std::endl
        << rhs.columns[3];
    return os;
}

std::string 
to_string(const matrix4& rhs)
{
    std::string result;
    result += to_string(rhs.columns[0]);
    result += '\n';
    result += to_string(rhs.columns[1]);
    result += '\n';
    result += to_string(rhs.columns[2]);
    result += '\n';
    result += to_string(rhs.columns[3]);
    return result;
}
