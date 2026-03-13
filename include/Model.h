//
// Created by desktop on 26. 3. 11..
//

#ifndef TOYENGINE_MODEL_H
#define TOYENGINE_MODEL_H

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include "Geometry.h"

class Model {
public:
    Model(const char* filename) {
        std::ifstream in;
        in.open(filename, std::ifstream::in);
        if (in.fail()) {
            std::cerr << "Error opening file " << filename << std::endl;
            return;
        }
        std::string line;
        while (!in.eof()) {
            std::getline(in, line);
            std::istringstream iss(line.c_str());
            char trash;
            if (!line.compare(0, 2, "v ")) {
                iss >> trash;
                Vec3 v;
                iss >> v.x >> v.y >> v.z;
                verts_.push_back(v);
            }else if (!line.compare(0, 2, "f ")) {
                std::vector<int> f;
                int itrash, idx;
                iss >> trash;
                while (iss >> idx >> trash >> itrash >> trash >> itrash) {
                    f.push_back(idx-1);
                }
                faces_.push_back(f);
            }
        }
    }

    int nverts() { return (int)verts_.size(); }
    int nfaces() { return (int)faces_.size(); }
    Vec3 vert(int i) { return verts_[i]; };
    std::vector<int> face(int idx) { return faces_[idx]; }

private:
    std::vector<Vec3> verts_;
    std::vector<std::vector<int> > faces_;
};

#endif //TOYENGINE_MODEL_H