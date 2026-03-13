//
// Created by desktop on 26. 1. 31..
//

#ifndef TOYENGINE_GEOMETRY_H
#define TOYENGINE_GEOMETRY_H

struct Vec3 {
    double x, y, z;
    Vec3(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) {}
    Vec3 operator+(const Vec3& v) const {return {x + v.x, y + v.y, z + v.z};}
    Vec3 operator-(const Vec3& v) const {return {x - v.x, y - v.y, z - v.z};}
    Vec3 operator*(const Vec3& v) const {return {x * v.x, y * v.y, z * v.z};}
    Vec3 operator*(const double& s) const {return {x * s, y * s, z * s};}
    Vec3 operator/(const double& s) const {return {x / s, y / s, z / s};}
};

struct Vec2i {
    int x, y;
    Vec2i() : x(0), y(0) {}
    Vec2i(const int _x, const int _y) : x(_x), y(_y) {}
};

using Color = Vec3;
using Point = Vec3;

static Vec3 crossV3(const Vec3& a, const Vec3& b) {
    return {a.y * b.z - a.z * b.y, -1 * (a.x * b.z) + a.z * b.x, a.x * b.y - a.y * b.x};
}

static Vec3 barycentric(const Vec3 *pts, const Vec2i P) {
    Vec3 vx = (Vec3(pts[1].x - pts[0].x, pts[2].x - pts[0].x, pts[0].x - P.x));
    Vec3 vy = (Vec3(pts[1].y - pts[0].y, pts[2].y - pts[0].y, pts[0].y - P.y));

    Vec3 u = crossV3(vx, vy);

    double u_val = u.x / u.z;
    double v_val = u.y / u.z;

    return {1. - (u_val + v_val), u_val, v_val};
}

#endif //TOYENGINE_GEOMETRY_H