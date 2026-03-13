//
// Created by desktop on 26. 1. 31..
//

#ifndef TOYENGINE_CANVAS_H
#define TOYENGINE_CANVAS_H
#include <limits>
#include <string>
#include <vector>
#include "Geometry.h"

class Canvas {
private:
    const int width, height;
    std::vector<Vec3> pixels;
    std::vector<double> zbuffer;
public:
    Canvas(int w, int h) : width(w), height(h), pixels(w * h, Color(0, 0, 0)),
        zbuffer(w * h, -std::numeric_limits<double>::max()) { }
    void set_pixel(int x, int y, const Vec3& color, double z = 1e-9);
    void save_ppm(const std::string& filename);
    int getWidth() const { return width; }
    int getHeight() const { return height; }
};

#endif //TOYENGINE_CANVAS_H