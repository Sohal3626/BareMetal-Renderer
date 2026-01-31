#include "include/Canvas.h"
#include "include/Renderer.h"

int main() {
    Canvas cv(1920, 1080);
    int x0 = 900;
    int x1 = 250;
    int y0 = 20;
    int y1 = 600;
    draw_line(cv, x0, y0, x1, y1, Color(1, 0, 0));
    cv.save_ppm("output.ppm");
    return 0;
}