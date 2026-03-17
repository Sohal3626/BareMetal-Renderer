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
    Material mat("../3Dobjects/african_head.mtl");
    DataBuilder db;
    std::vector<RenderUnit> units = db.build(md, mat, "./Textures");
    Matrix44 tp{};

    Renderer::draw_model(cv1, units, tp);


    cv.save_ppm("output1.ppm");
    cv1.save_ppm("output2.ppm");

    return 0;
}
