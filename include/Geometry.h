//
// Created by desktop on 26. 1. 31..
//

#ifndef TOYENGINE_GEOMETRY_H
#define TOYENGINE_GEOMETRY_H

struct Vec3 {
    float x, y, z;
    Vec3(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}
};

using Color = Vec3;
using Point = Vec3;

#endif //TOYENGINE_GEOMETRY_H