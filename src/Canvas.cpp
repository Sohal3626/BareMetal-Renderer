//
// Created by desktop on 26. 1. 31..
//

#include "../include/Canvas.h"
#include <fstream>
#include <iostream>

using namespace std;

void Canvas::set_pixel(int x, int y, const Vec3 &color) {
    if (x < 0 || x >= width || y < 0 || y >= height) return;
    pixels[y * width + x] = color;
}

void Canvas::save_ppm(const std::string &filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Unable to open file" << std::endl;
        return;
    }
    file << "P3\n" << width << " " << height << "\n255\n";

    for (const auto& p : pixels) {
        file << (int)(255.99 * p.x) << " "
            << (int)(255.99 * p.y) << " "
            << (int)(255.99 * p.z) << "\n";
    }
}