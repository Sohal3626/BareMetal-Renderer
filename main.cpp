#include <iostream>

#include "include/Canvas.h"
#include "include/Renderer.h"
#include "include/Geometry.h"
#include "include/Model.h"

int main() {
    Canvas cv(1920, 1080);
    Canvas cv2(1920, 1080);

    Model md("D:/coding/toyEngine/3Dobjects/african_head.obj");

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

    Renderer::fill_triangle(cv, Vec2i(x0, y0), Vec2i(x1, y1), Vec2i(x2, y2), c1, c2, c3 );
    cv.save_ppm("output.ppm");

    // 아래는 테스트
    for (int i=0; i<md.nfaces(); i++) {
        std::vector<int> face = md.face(i);

        int w = cv2.getWidth();
        int h = cv2.getHeight();

        Vec2i pts[3];

        for (int j=0; j<3; j++) {
            Vec3 v = md.vert(face[j]);

            int sx = (int)((v.x + 1.) * w / 2.);
            int sy = (int)((v.y + 1.) * h / 2.);

            pts[j] = Vec2i(sx, sy);
        }

        Renderer::fill_triangle(cv2, pts[0], pts[1], pts[2], c1, c2, c3 );
        /*
        for (int j=0; j<3; j++) {


            Vec3 v0 = md.vert(face[j]);
            Vec3 v1 = md.vert(face[(j+1)%3]);

            int x0 = (v0.x + 1.) * w / 2.;
            int y0 = (v0.y + 1.) * h / 2.;
            int x1 = (v1.x + 1.) * w / 2.;
            int y1 = (v1.y + 1.) * h / 2.;

            Renderer::draw_line(cv2, x0, y0, x1, y1, Color(1, 1, 1));
        }
        */
    }
    cv2.save_ppm("output2.ppm");

    return 0;
}