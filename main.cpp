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

    Renderer::fill_triangle(cv, Vec3(x0, y0), Vec3(x1, y1), Vec3(x2, y2), c1, c2, c3 );
    cv.save_ppm("output.ppm");


    float min_x = 1e10, max_x = -1e10;
    float min_y = 1e10, max_y = -1e10;

    for (int i=0; i<md.nverts(); i++) {
        Vec3 v = md.vert(i);
        if (v.x < min_x) min_x = v.x;
        if (v.x > max_x) max_x = v.x;
        if (v.y < min_y) min_y = v.y;
        if (v.y > max_y) max_y = v.y;
    }

    float model_width = max_x - min_x;
    float model_height = max_y - min_y;

    float scale = std::max(model_width, model_height);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    // 아래는 테스트
    for (int i=0; i<md.nfaces(); i++) {
        std::vector<int> face = md.face(i);

        int w = cv2.getWidth();
        int h = cv2.getHeight();

        Vec3 pts[3];

        for (int j=0; j<3; j++) {
            Vec3 v = md.vert(face[j]);

            // 전체 화면 중앙에 배치하기 위한 오프셋(Margin) 계산
            // (v.x - min_x) / scale 은 0~1 사이의 값이 나옵니다.
            int sx = (int)(((v.x - min_x) / scale) * (w - 1));
            int sy = (int)((1.0 - (v.y - min_y) / scale) * (h - 1));


            pts[j] = Vec3(sx, sy, v.z);
        }

        Color c1(dis(gen), dis(gen), dis(gen));

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