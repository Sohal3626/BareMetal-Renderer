#include <iostream>
#include <fstream>

int main() {
    std::ofstream file("output.ppm");

    if (!file.is_open()) {
        std::cerr << "Unable to open file" << std::endl;
        return 1;
    }
    file << "P3\n" << "256 256\n" << "255\n";

    int b = 63;
    for (int y = 0; y < 256; y++) {
        for (int x = 0; x < 256; x++) {
            int r = (256-x); int g = y;
            file << r << ' ' << g << ' ' << b << '\n';
        }
    }

    return 0;
}