#include "include/Canvas.h"
#include "include/Renderer.h"
#include "include/Geometry.h"

int main() {
    Canvas cv(1920, 1080);
    int x0 = 900;
    int y0 = 20;
    int x1 = 950;
    int y1 = 600;
    int x2 = 560;
    int y2 = 990;
    //Renderer::draw_line(cv, x0, y0, x1, y1, Color(1, 0, 0));
    Renderer::fill_triangle(cv, Vec2i(x0, y0), Vec2i(x1, y1), Vec2i(x2, y2), Color(1, 0, 0));
    cv.save_ppm("output.ppm");
    return 0;
}