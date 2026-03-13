//
// Created by desktop on 26. 1. 31..
//
#include <iostream>
#include "../include/Canvas.h"
#include "../include/Renderer.h"
#include <algorithm>

#include <math.h>

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

void Renderer::fill_triangle(Canvas& canvas, Vec2i p0, Vec2i p1, Vec2i p2, Color c0, Color c1, Color c2) {
    int minX = min({p0.x, p1.x, p2.x});
    int minY = min({p0.y, p1.y, p2.y});
    int maxX = max({p0.x, p1.x, p2.x});
    int maxY = max({p0.y, p1.y, p2.y});

    minX = max(0, minX);
    minY = max(0, minY);
    maxX = min(canvas.getWidth() - 1, maxX);
    maxY = min(canvas.getHeight() - 1, maxY);

    for (int x = minX; x <= maxX; x++) {
        for (int y = minY; y <= maxY; y++) {
            Vec2i p[3] = {p0, p1, p2};
            Vec3 bc = barycentric(p, Vec2i(x, y));

            if (bc.x < 0 || bc.y < 0 || bc.z < 0) continue;

            if (bc.x >= 0 && bc.y >= 0 && bc.z >= 0) {
                double r = c0.x * bc.x + c1.x * bc.y + c2.x * bc.z;
                double g = c0.y * bc.x + c1.y * bc.y + c2.y * bc.z;
                double b = c0.z * bc.x + c1.z * bc.y + c2.z * bc.z;

                canvas.set_pixel(x, y, Color(r, g, b));
            }
        }
    }
}