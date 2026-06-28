#pragma once
#include <cmath>

/// @brief vector operations
struct Vec3
{
    double x{0.0};
    double y{0.0};
    double z{0.0};

    // add two vectors
    Vec3 operator+(const Vec3 other) const
    {
        return {x + other.x, y + other.y, z + other.z};
    }

    // subtract two vectors
    Vec3 operator-(const Vec3 other) const
    {
        return {x - other.x, y - other.y, z - other.z};
    }

    // multiply by a scalar
    Vec3 operator*(double scalar) const
    {
        return {x * scalar, y * scalar, z * scalar};
    }

    // distance to a point
    double distanceTo(const Vec3 other) const
    {
        double d_x = abs(other.x - x);
        double d_y = abs(other.y - y);
        double d_z = abs(other.z - z);

        return std::sqrt(d_x * d_x + d_y * d_y + d_z * d_z);
    }

    // length of vector from origin
    double length() const
    {
        return std::sqrt(x * x + y * y + z * z);
    }
};
