//
// Created by desktop on 26. 1. 31..
//
#include <iostream>
#include "../include/Canvas.h"
#include "../include/Renderer.h"
#include <algorithm>
#include <cmath>

using namespace std;

void Renderer::draw_line(Canvas& canvas, int x0, int y0, int x1, int y1, Color color) {
    bool steep = abs(y1 - y0) > abs(x1 - x0);
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

void Renderer::draw_horizontal_line(Canvas& cv, int x1, int x2, int y, Color c1, Color c2) {
    int width = x2 - x1;
    if (width <= 0) {
        cv.set_pixel(x1, y, c1);
        return;
    }

    for (int i = x1; i <= x2; i++) {
        double t = (double)(i - x1) / width;
        Color tc = c1 + (c2 - c1) * t;
        cv.set_pixel(i, y, tc);
    }
}

void Renderer::fill_triangle(Canvas& canvas, Vec<3> p0, Vec<3> p1, Vec<3> p2, Color c0, Color c1, Color c2) {
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
            Vec<3> bc = barycentric(p, {(double)x , (double)y});

            if (bc[0] < 0 || bc[1] < 0 || bc[2] < 0) continue;
            double z = p0[2] * bc[0] + p1[2] * bc[1] + p2[2] * bc[2];
            double r = bc[0] * c0[0] + bc[1] * c1[0] + bc[2] * c2[0];
            double g = bc[0] * c0[1] + bc[1] * c1[1] + bc[2] * c2[1];
            double b = bc[0] * c0[2] + bc[1] * c1[2] + bc[2] * c2[2];
            Color color = {r, g, b};

            canvas.set_pixel(x, y, color, z);
        }
    }
}

void Renderer::draw_model(Canvas& canvas, Model& model, Vec<3> light_dir, double camera_z) {
    Matrix44 projection;
    projection.m[14] = -1.0 / camera_z;

    float p_min_x = 1e10, p_max_x = -1e10;
    float p_min_y = 1e10, p_max_y = -1e10;

    for (int i=0; i<model.nverts(); i++) {
        Vec<3> v = model.vert(i);

        if (v[0] < p_min_x) p_min_x = v[0];
        if (v[0] > p_max_x) p_max_x = v[0];
        if (v[1] < p_min_y) p_min_y = v[1];
        if (v[1] > p_max_y) p_max_y = v[1];
    }

    float p_scale = std::max(p_max_x - p_min_x, p_max_y - p_min_y);
    int w = canvas.getWidth();
    int h = canvas.getHeight();

    for (int i=0; i<model.nfaces(); i++) {
        vector<int> face = model.face(i);
        Vec<3> world[3];
        Vec<3> screen[3];

        for (int j=0; j<3; j++) {
            Vec<3> v = model.vert(face[j]);
            world[j] = v;

            Vec<4> v4 = {v[0], v[1], v[2], 1.};
            Vec<3> pv = projection * v4;

            double sx = (int)(((pv[0] - p_min_x) / p_scale) * (w - 1));
            double sy = (int)((1.0 - (pv[1] - p_min_y) / p_scale) * (h - 1));

            screen[j] = {sx, sy, pv[2]};
        }

        Vec<3> edge1 = world[2] - world[0];
        Vec<3> edge2 = world[1] - world[0];
        Vec<3> cEdge = crossV3(edge1, edge2);
        double norm = sqrt(cEdge[0] * cEdge[0] + cEdge[1] * cEdge[1] + cEdge[2] * cEdge[2]);
        if (norm > 0) cEdge = cEdge / norm;

        double intensity = dotV3(cEdge, light_dir);
        if (intensity < 0) intensity = abs(intensity);

        Color c = {intensity, intensity, intensity};
        Renderer::fill_triangle(canvas, screen[0], screen[1], screen[2], c, c, c);
    }
}