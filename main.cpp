#include "include/Canvas.h"
#include "include/Renderer.h"
#include "include/Geometry.h"

int main() {
    Canvas cv(1920, 1080);
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
    return 0;
}