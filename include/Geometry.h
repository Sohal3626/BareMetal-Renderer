#include <utility>

//
// Created by desktop on 26. 1. 31..
//

#ifndef TOYENGINE_GEOMETRY_H
#define TOYENGINE_GEOMETRY_H

template <int N>
class Vec {
private:
    std::vector<double> data;
public:
    Vec() : data(N) {for ( int i=0; i<N; i++) data[i] = 0;}
    Vec(std::vector<double> v) : data(std::move(v)) {};
    Vec(std::initializer_list<double> v) : data(N) {
        auto it  = v.begin();
        for (int i=0; i<N && it != v.end(); ++i, ++it) data[i] = *it;
    };
    Vec<N> operator+(const Vec<N>& v) const {
        Vec<N> r;
        for ( int i=0; i<N; i++) r.data[i] = data[i] + v.data[i];
        return r;
    }
    Vec<N> operator-(const Vec<N>& v) const {
        Vec<N> r;
        for ( int i=0; i<N; i++) r.data[i] = data[i] - v.data[i];
        return r;
    }
    Vec<N> operator*(const double& s) const {
        Vec<N> r;
        for ( int i=0; i<N; i++) r.data[i] = data[i] * s;
        return r;
    }
    Vec<N> operator/(const double& s) const {
        Vec<N> r;
        for ( int i=0; i<N; i++) r.data[i] = data[i] / s;
        return r;
    }
    double& operator[](const int i) { return data[i]; }
};

using Color = Vec<3>;
using Point = Vec<3>;

struct Matrix44 {
    std::vector<double> m;
    Matrix44() : m(16) {
        for (int i=0; i<16; i++) {
                m[i] = 0.;
            }
        m[0] = m[5] = m[10] = m[15] = 1.;
    }

    double& idx(const int i, const int j) {
        return m[i * 4 + j];
    }

    Vec<4> operator*(Vec<4>& v) {
        double res[4];
        for (int i=0; i<4; i++) {
            res[i] = idx(i, 0) * v[0] + idx(i, 1) * v[1] + idx(i, 2) * v[2] + idx(i, 3) * v[3];
        }
        return {res[0], res[1], res[2], res[3]};
    }

    Vec<3> perspective(Vec<4>& v) {
        Vec<4> res = (*this) * v;
        if (std::abs(res[3]) > 1e-9) return { res[0]/res[3], res[1]/res[3], res[2]/res[3] };
        return {res[0], res[1], res[2]};
    }
};


static Vec<3> crossV3(Vec<3> &a, Vec<3> b) {
    return {a[1] * b[2] - a[2] * b[1], -1 * (a[0] * b[2]) + a[2] * b[0], a[0] * b[1] - a[1] * b[0]};
}

static double dotV3(Vec<3>& a, Vec<3>& b) {
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

static Vec<3> barycentric(Vec<3> *pts, Vec<2> p) {
    Vec<3> vx = {pts[1][0] - pts[0][0], pts[2][0] - pts[0][0], pts[0][0] - p[0]};
    Vec<3> vy = {pts[1][1] - pts[0][1], pts[2][1] - pts[0][1], pts[0][1] - p[1]};

    Vec<3> u = crossV3(vx, vy);

    double u_val = u[0] / u[2];
    double v_val = u[1] / u[2];

    return {1. - (u_val + v_val), u_val, v_val};
}

#endif //TOYENGINE_GEOMETRY_H