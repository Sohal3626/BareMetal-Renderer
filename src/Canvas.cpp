//
// Created by desktop on 26. 1. 31..
//

#include "../include/Canvas.h"
#include <fstream>
#include <iostream>

using namespace std;

void Canvas::set_pixel(int x, int y, const Color &color, double z) {
    if (x < 0 || x >= width || y < 0 || y >= height) return;
    if (int idx = x + y * width; zbuffer[idx] < z) {
        zbuffer[idx] = z;
        pixels[idx] = color;
    }
}

void Canvas::save_ppm(const std::string &filename) const
{
    std::ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Unable to open file" << std::endl;
        return;
    }
    file << "P3\n" << width << " " << height << "\n255\n";

    for (Point p : pixels) {
        file << (int)(255.99 * p[0]) << " "
            << (int)(255.99 * p[1]) << " "
            << (int)(255.99 * p[2]) << "\n";
    }
}
