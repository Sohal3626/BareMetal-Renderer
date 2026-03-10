//
// Created by desktop on 26. 1. 31..
//
#include <iostream>
#include "../include/Canvas.h"
#include "../include/Renderer.h"

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

void Renderer::fill_triangle(Canvas& canvas, Vec2i p0, Vec2i p1, Vec2i p2, Color c0, Color c1, Color c2){
    if (p0.y > p1.y) {
        swap(p0, p1);
        swap(c0, c1);
    }
    if (p1.y > p2.y) {
        swap(p1, p2);
        swap(c1, c2);
    }
    if (p0.y > p1.y) {
        swap(p0, p1);
        swap(c0, c1);
    }

    if (p0.y == p2.y) return;

    for (int y = p0.y; y < p1.y; y++) {
        double t_short = (double)(y - p0.y) / (p1.y - p0.y + 1e-6);
        double t_long = (double)(y - p0.y) / (p2.y - p0.y + 1e-6);

        int x_size1 = p0.x + ((p1.x - p0.x) * t_short);
        int x_size2 = p0.x + ((p2.x - p0.x) * t_long);

        Color c_short = c0 + (c1 - c0) * t_short;
        Color c_long = c0 + (c2 - c0) * t_long;

        if (x_size1 > x_size2) {
            swap(x_size1, x_size2);
            swap(c_short, c_long);
        }

        draw_horizontal_line(canvas, x_size1, x_size2, y, c_short, c_long);
    }

    for (int y = p1.y; y <= p2.y; y++) {
        double t_short = (double)(y - p1.y) / (p2.y - p1.y + 1e-6);
        double t_long = (double)(y - p0.y) / (p2.y - p0.y + 1e-6);

        int x_size1 = p1.x + ((p2.x - p1.x) * t_short);
        int x_size2 = p0.x + ((p2.x - p0.x) * t_long);

        Color c_short = c1 + (c2 - c1) * t_short;
        Color c_long = c0 + (c2 - c0) * t_long;

        if (x_size1 > x_size2) {
            swap(x_size1, x_size2);
            swap(c_short, c_long);
        }

        draw_horizontal_line(canvas, x_size1, x_size2, y, c_short, c_long);
    }
}