//
// Created by desktop on 26. 1. 31..
//
#include <iostream>
#include "../include/Canvas.h"

using namespace std;

void draw_line(Canvas& canvas, int x0, int y0, int x1, int y1, Color color) {
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
