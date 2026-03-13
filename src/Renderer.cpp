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

void Renderer::fill_triangle(Canvas& canvas, Vec3 p0, Vec3 p1, Vec3 p2, Color c0, Color c1, Color c2) {
    int minX = (int)floor(min({p0.x, p1.x, p2.x}));
    int minY = (int)floor(min({p0.y, p1.y, p2.y}));
    int maxX = (int)floor(max({p0.x, p1.x, p2.x}));
    int maxY = (int)floor(max({p0.y, p1.y, p2.y}));

    minX = max(0, minX);
    minY = max(0, minY);
    maxX = min(canvas.getWidth() - 1, maxX);
    maxY = min(canvas.getHeight() - 1, maxY);

    Vec3 p[3] = {p0, p1, p2};
    for (int x = minX; x <= maxX; x++) {
        for (int y = minY; y <= maxY; y++) {
            Vec3 bc = barycentric(p, Vec2i(x, y));

            if (bc.x < 0 || bc.y < 0 || bc.z < 0) continue;
            double z = p0.z * bc.x + p1.z * bc.y + p2.z * bc.z;
            double r = bc.x * c0.x + bc.y * c1.x + bc.z * c2.x;
            double g = bc.x * c0.y + bc.y * c1.y + bc.z * c2.y;
            double b = bc.x * c0.z + bc.y * c1.z + bc.z * c2.z;
            Color color = Color(r, g, b);

            canvas.set_pixel(x, y, color, z);
        }
    }
}

void Renderer::draw_model(Canvas& canvas, Model& model, Vec3 light_dir) {
    float min_x = 1e10, max_x = -1e10;
    float min_y = 1e10, max_y = -1e10;

    for (int i=0; i<model.nverts(); i++) {
        Vec3 v = model.vert(i);
        if (v.x < min_x) min_x = v.x;
        if (v.x > max_x) max_x = v.x;
        if (v.y < min_y) min_y = v.y;
        if (v.y > max_y) max_y = v.y;
    }

    for (int i=0; i<model.nfaces(); i++) {
        vector<int> face = model.face(i);

        Vec3 world[3];
        Vec3 screen[3];

        for (int j=0; j<3; j++) {
            world[j] = model.vert(face[j]);
        }

        float model_width = max_x - min_x;
        float model_height = max_y - min_y;

        float scale = std::max(model_width, model_height);

        int w = canvas.getWidth();
        int h = canvas.getHeight();

        for (int j=0; j<3; j++) {
            Vec3 v = model.vert(face[j]);

            int sx = (int)(((v.x - min_x) / scale) * (w - 1));
            int sy = (int)((1.0 - (v.y - min_y) / scale) * (h - 1));

            screen[j] = Vec3(sx, sy, world[j].z);
        }

        Vec3 edge1 = world[2] - world[0];
        Vec3 edge2 = world[1] - world[0];
        Vec3 cEdge = crossV3(edge1, edge2);
        double norm = sqrt(cEdge.x * cEdge.x + cEdge.y * cEdge.y + cEdge.z * cEdge.z);
        if (norm > 0) cEdge = cEdge / norm;
        double intensity = dotV3(cEdge, light_dir);
        if (intensity <= 0) continue;

        Color c = Color(intensity, intensity, intensity);
        Renderer::fill_triangle(canvas, screen[0], screen[1], screen[2], c, c, c);
    }
}