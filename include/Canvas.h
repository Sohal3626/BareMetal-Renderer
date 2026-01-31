//
// Created by desktop on 26. 1. 31..
//

#ifndef TOYENGINE_CANVAS_H
#define TOYENGINE_CANVAS_H
#include <string>
#include <vector>
#include "Geometry.h"

class Canvas {
private:
    const int width, height;
    std::vector<Vec3> pixels;
public:
    Canvas(int w, int h) : width(w), height(h), pixels(w * h) {}
    void set_pixel(int x, int y, const Vec3& color);
    void save_ppm(const std::string& filename);
};

#endif //TOYENGINE_CANVAS_H