//
// Created by desktop on 26. 1. 31..
//

#include <iostream>
#include "../include/Canvas.h"
#include "../include/Renderer.h"
#include "../include/RenderData.h"
#include <algorithm>
#include <cmath>

using namespace std;

void Renderer::draw_line(Canvas& canvas, int x0, int y0, int x1, int y1, const Color& color) {
    const bool steep = abs(y1 - y0) > abs(x1 - x0);
    if (steep) {
        swap(x0, y0);
        swap(x1, y1);
    }

    if (x0 > x1) {
        swap(x0, x1);
        swap(y0, y1);
    }

    int dx = x1 - x0;
    int dy = abs(y1 - y0);

    const int ystep = (y0 < y1) ? 1 : -1;
    int y = y0;
    int eps = 0;

    for (int x = x0; x <= x1; x++) {
        if (steep) canvas.set_pixel(y, x, color);
        else       canvas.set_pixel(x, y, color);

        eps += dy;
        if (eps >= dx) {
            y += ystep;
            eps -= dx;
        }
    }
}

void Renderer::draw_horizontal_line(Canvas& cv, const int x1, const int x2, const int y,
    const Color& c1, const Color& c2) {
    const int width = x2 - x1;
    if (width <= 0) {
        cv.set_pixel(x1, y, c1);
        return;
    }

    for (int i = x1; i <= x2; i++) {
        double t = static_cast<double>(i - x1) / width;
        Color tc = c1 + (c2 - c1) * t;
        cv.set_pixel(i, y, tc);
    }
}

void Renderer::fill_triangle(Canvas& canvas, Vec<3> p0, Vec<3> p1, Vec<3> p2,
        const Texture& texture, double intensity, Vec<2>* uvs) {
    int minX = (int)floor(min({p0[0], p1[0], p2[0]}));
    int minY = (int)floor(min({p0[1], p1[1], p2[1]}));
    int maxX = (int)floor(max({p0[0], p1[0], p2[0]}));
    int maxY = (int)floor(max({p0[1], p1[1], p2[1]}));

    minX = max(0, minX);
    minY = max(0, minY);
    maxX = min(canvas.getWidth() - 1, maxX);
    maxY = min(canvas.getHeight() - 1, maxY);

    Vec<3> p[3] = {p0, p1, p2};
    for (int x = minX; x <= maxX; x++) {
        for (int y = minY; y <= maxY; y++) {
            Vec<3> bc = barycentric(p, {static_cast<double>(x) , static_cast<double>(y)});

            if (bc[0] < 0 || bc[1] < 0 || bc[2] < 0 || isnan(bc[0]) || isnan(bc[1]) || isnan(bc[2])) continue;
            const double z = p0[2] * bc[0] + p1[2] * bc[1] + p2[2] * bc[2];

            const double u = uvs[0][0] * bc[0] + uvs[1][0] * bc[1] + uvs[2][0] * bc[2];
            const double v = uvs[0][1] * bc[0] + uvs[1][1] * bc[1] + uvs[2][1] * bc[2];

            Color color = texture.get_color(u, v);
            canvas.set_pixel(x, y, color*intensity, z);
        }
    }
}

Matrix44 Renderer::transfrom(const double p_scale,
    const double angle_x, const double angle_y, const double angle_z, const double camera_z) {
    Matrix44 scale;
    scale.idx(0,0) = 1.0/p_scale; scale.idx(1,1) = 1.0/p_scale; scale.idx(2,2) = 1.0/p_scale;

    Matrix44 rotationY;
    rotationY.m[0] = cos(angle_y);
    rotationY.m[2] = sin(angle_y);
    rotationY.idx(2, 0) = -sin(angle_y);
    rotationY.idx(2, 2) = cos(angle_y);

    Matrix44 rotationX;
    rotationX.idx(1, 1) =  cos(angle_x);
    rotationX.idx(1, 2) = -sin(angle_x);
    rotationX.idx(2, 1) =  sin(angle_x);
    rotationX.idx(2, 2) =  cos(angle_x);

    Matrix44 rotationZ;
    rotationZ.m[0] = cos(angle_z);
    rotationZ.m[1] = -sin(angle_z);
    rotationZ.idx(1, 0) = sin(angle_z);
    rotationZ.idx(1, 1) = cos(angle_z);

    Matrix44 proj;
    proj.m[14] = -1.0 / camera_z;

    return proj * rotationX * rotationY * rotationZ * scale;
}

double Renderer::get_intensity(const Vec<3> *pts, Vec<3> light_dir) {
    Vec<3> edge1 = pts[2] - pts[0];
    Vec<3> edge2 = pts[1] - pts[0];
    Vec<3> cEdge = crossV3(edge1, edge2);
    if (const double norm = sqrt(cEdge[0] * cEdge[0] + cEdge[1] * cEdge[1] + cEdge[2] * cEdge[2]); norm > 0)
        cEdge = cEdge / norm;

    const double light_norm =
        sqrt(light_dir[0] * light_dir[0] + light_dir[1] * light_dir[1] + light_dir[2] * light_dir[2]);
    light_dir = light_dir / light_norm;
    const double intensity = dotV3(cEdge, light_dir);
    if (intensity < 0) return 0;
    return intensity;
}

void Renderer::draw_model(Canvas &canvas, const vector<RenderUnit>& units,
    const Matrix44 &transform, const Vec<3>& light_dir) {
    int w = canvas.getWidth();
    int h = canvas.getHeight();

    for (const auto& unit : units) {
        Texture* curr_tex = unit.tex.get();
        for (size_t i = 0; i < unit.buffer.size(); i+=24) {
            Vec<3> screen[3];
            Vec<2> uvs[3];
            Vec<3> normal[3]; // vn을 이용한 플랫 쉐이딩은 나중에 구현
            Vec<3> world[3];
            for (int j=0; j<3; j++) {
                int tmp = i + (j*8);
                if (tmp + 7 >= unit.buffer.size()) break;
                Vec<4> v4 = {unit.buffer[tmp], unit.buffer[tmp+1], unit.buffer[tmp+2], 1.};
                uvs[j] = {unit.buffer[tmp+3], unit.buffer[tmp+4]};
                normal[j] = {unit.buffer[tmp+5], unit.buffer[tmp+6], unit.buffer[tmp+7]};
                Vec<4> rv = transform * v4;
                world[j] = {rv[0], rv[1], rv[2]};
                Vec<3> pv = transform.perspective(rv);

                double sx = static_cast<int>((pv[0] + 1.0) * 0.5 * (w - 1));
                double sy = static_cast<int>((1.0 - (pv[1] + 1.0) * 0.5) * (h - 1));
                screen[j] = {sx, sy, pv[2]};
            }
            const double intensity = get_intensity(world, light_dir);
            if (intensity <= 0) continue;
            fill_triangle(canvas, screen[0], screen[1], screen[2], *curr_tex ,intensity, uvs);

        }
    }
}