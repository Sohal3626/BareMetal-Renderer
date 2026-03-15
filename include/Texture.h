//
// Created by desktop on 26. 3. 15..
//

#ifndef TOYENGINE_TEXTURE_H
#define TOYENGINE_TEXTURE_H

#include "stb_image.h"
#include "Geometry.h"
#include <iostream>

class Texture {
public:
    int width, height, channels;
    unsigned char* data;

    Texture(const char* filename) : width(0), height(0), channels(0), data(NULL) {
        stbi_set_flip_vertically_on_load(true);
        data = stbi_load(filename, &width, &height, &channels, 3);

        if (!data) {
            std::cerr << "Missing file -> " << filename << std::endl;
        }
    }

    ~Texture() {
        if (data) stbi_image_free(data);
    }

    Color get_color(double u, double v) {
        if (!data) {
            // 아니 시발 왜 중괄호 생성이 안되는거야 ㅅㅂ
            Color c;
            c[0] = 1.0;
            c[1] = 0.0;
            c[2] = 1.0;
            return c;
        }

        int x = (int)(u * (width - 1));
        int y = (int)(v * (height - 1));

        if (x < 0) x = 0; if (x >= width) x = width - 1;
        if (y < 0) y = 0; if (y >= height) y = height - 1;

        int idx = (y * width + x) * channels;

        Color c;

        // 그래서 중괄호 생성 왜안되는거임?
        c[0] = data[idx] / 255.0;
        c[1] = data[idx+1] / 255.0;
        c[2] = data[idx+2] / 255.0;
        return c;
    }
};


#endif //TOYENGINE_TEXTURE_H