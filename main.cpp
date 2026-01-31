#include "include/Canvas.h"

int main() {
    Canvas cv(256, 256);
    for (int i = 0; i < 256; i++) {
        for (int j = 0; j < 256; j++) {
            cv.set_pixel(i, j, Vec3((float)i/255.f, (float)j/255.f, 0.0f));
        }
    }
    cv.save_ppm("output.ppm");
    return 0;
}