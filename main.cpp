#include <iostream>
#include <random>

#include "include/Canvas.h"
#include "include/Renderer.h"
#include "include/Geometry.h"
#include "include/Model.h"

int main() {
    Canvas cv(1920, 1080);
    Canvas cv1(1420, 1480);
    Canvas cv2(1420, 1520);

    Model md("../3Dobjects/african_head.obj");

    Texture tx("../Textures/african_head.jpg");
    Texture tx1("../Textures/african_head2.tga");
    Texture tx2("temp");

    double x0 = 900;
    double y0 = 20;

    double x1 = 500;
    double y1 = 450;

    double x2 = 1800;
    double y2 = 990;

    Renderer::draw_model(cv, md, tx1, {0, 0, 1}, 3.0, 0.0, 0.0);
    //Renderer::draw_model(cv1, md2, tx2, {0, 0, -1}, 3.0, 0.0, 0.0);

    cv.save_ppm("output1.ppm");
    cv1.save_ppm("output2.ppm");

    return 0;
}
