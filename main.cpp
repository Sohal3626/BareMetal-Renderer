#include <iostream>
#include <random>

#include "include/Canvas.h"
#include "include/Renderer.h"
#include "include/Geometry.h"
#include "include/Model.h"

int main() {
    Canvas cv(1920, 1080);
    Canvas cv2(1920, 1080);

    Model md("D:/coding/toyEngine/3Dobjects/african_head.obj");
    Model md2("D:/coding/toyEngine/3Dobjects/Miyu.obj");
    //md = md2;

    int x0 = 900;
    int y0 = 20;

    int x1 = 500;
    int y1 = 450;

    int x2 = 1800;
    int y2 = 990;

    Color c1 = Color(1, 0, 0);
    Color c2 = Color(0, 1, 0);
    Color c3 = Color(0, 0, 1);

    //Renderer::draw_line(cv, 200, 5, 900, 900, Color(0, 0, 1));
    Renderer::fill_triangle(cv, Vec3(x0, y0), Vec3(x1, y1), Vec3(x2, y2),
        c1, c2, c3);
    Renderer::draw_model(cv2, md);

    cv.save_ppm("output.ppm");
    cv2.save_ppm("output2.ppm");

    return 0;
}