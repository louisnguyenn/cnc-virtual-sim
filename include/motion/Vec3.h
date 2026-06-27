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
    // TODO

    // multiply by a scalar
    // TODO

    // distance between two points
    // TODO

    // length of vector from origin
    // TODO
}