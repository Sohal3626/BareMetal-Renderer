//
// Created by desktop on 26. 1. 31..
//

#ifndef TOYENGINE_GEOMETRY_H
#define TOYENGINE_GEOMETRY_H

struct Vec3 {
    double x, y, z;
    Vec3(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) {}
    Vec3 operator+(const Vec3& v) const {return Vec3(x + v.x, y + v.y, z + v.z);}
    Vec3 operator-(const Vec3& v) const {return Vec3(x - v.x, y - v.y, z - v.z);}
    Vec3 operator*(const Vec3& v) const {return Vec3(x * v.x, y * v.y, z * v.z);}
    Vec3 operator*(const double& s) const {return Vec3(x * s, y * s, z * s);}
    Vec3 operator/(const double& s) const {return Vec3(x / s, y / s, z / s);}
};

struct Vec2i {
    int x, y;
    Vec2i() : x(0), y(0) {}
    Vec2i(int _x, int _y) : x(_x), y(_y) {}
};

using Color = Vec3;
using Point = Vec3;

#endif //TOYENGINE_GEOMETRY_H