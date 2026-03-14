#include <iostream>
#include <random>

#include "include/Canvas.h"
#include "include/Renderer.h"
#include "include/Geometry.h"
#include "include/Model.h"

int main() {
    Canvas cv(1920, 1080);
    Canvas cv2(1420, 1480);
    Canvas cv3(1420, 1520);

    Model md("../3Dobjects/african_head.obj");

    double x0 = 900;
    double y0 = 20;

    double x1 = 500;
    double y1 = 450;

    double x2 = 1800;
    double y2 = 990;

    Color c1 = {1, 0, 0};
    Color c2 = {0, 1, 0};
    Color c3 = {0, 0, 1};

    Renderer::fill_triangle(cv, Vec<3>{x0, y0, 0.}, Vec<3>{x1, y1, 0.}, Vec<3>{x2, y2, 0},
        c1, c2, c3);
    Renderer::draw_model(cv2, md, {0, 0, -1}, 3.0, 0.0, 0.0);
    //Renderer::draw_model(cv3, md2, {0, 0, -1}, 3.0, 0.0, 0.0);

    cv.save_ppm("output.ppm");
    cv2.save_ppm("output2.ppm");
    cv3.save_ppm("output3.ppm");

    return 0;
}