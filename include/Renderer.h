//
// Created by desktop on 26. 1. 31..
//

#ifndef TOYENGINE_RENDERER_H
#define TOYENGINE_RENDERER_H
#include "Geometry.h"
#include "Canvas.h"
#include "Model.h"
#include "Texture.h"

class Renderer {
public:
    static void draw_line(Canvas& canvas, int x0, int y0, int x1, int y1, Color c1 = {0, 0, 0});
    static void fill_triangle(Canvas& canvas, Vec<3> p0, Vec<3> p1, Vec<3> p2,
        Texture& texture, double intensity, Vec<2>* uvs);
    static void draw_model(Canvas &canvas, Model &model, Texture &texture, Vec<3> light_dir = {0., 0., -1},
                           double camera_z = 3.0, double angleX = 0.0, double angleY = 0.0, double angleZ = 0.0);
private:
    static void draw_horizontal_line(Canvas& cv, int x1, int x2, int y, Color c1, Color c2);
};


#endif //TOYENGINE_RENDERER_H