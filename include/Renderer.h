//
// Created by desktop on 26. 1. 31..
//

#ifndef TOYENGINE_RENDERER_H
#define TOYENGINE_RENDERER_H
#include "Geometry.h"
#include "Canvas.h"
#include "Model.h"
#include "RenderData.h"
#include "Texture.h"

class Renderer {
public:
    static void draw_line(Canvas& canvas, int x0, int y0, int x1, int y1, const Color& c1 = {0, 0, 0});
    static void fill_triangle(Canvas& canvas, Vec<3> p0, Vec<3> p1, Vec<3> p2,
        const Texture& texture, double intensity, Vec<2>* uvs);
    static void draw_model(Canvas &canvas, const std::vector<RenderUnit>& units,
        const Matrix44 &transform, const Vec<3>& light_dir = {0, 0, -1});
    static Matrix44 transfrom(double p_scale,
        double angle_x = 0, double angle_y = 0, double angle_z = 0, double camera_z = 3);
private:
    static void draw_horizontal_line(Canvas& cv, int x1, int x2, int y, const Color& c1, const Color& c2);
    static double get_intensity(const Vec<3> *pts, Vec<3> light_dir);
};


#endif //TOYENGINE_RENDERER_H