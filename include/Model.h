//
// Created by desktop on 26. 3. 11..
//

#ifndef TOYENGINE_MODEL_H
#define TOYENGINE_MODEL_H

#include <algorithm>
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
                Vec<3> v;
                iss >> v[0] >> v[1] >> v[2];
                verts_.push_back(v);
            }else if (!line.compare(0, 2, "f ")) {
                std::vector<int> f;
                int idx, v_idx, t_idx, n_idx;
                iss >> trash;

                std::string segment;
                while (iss >> segment) {
                    std::replace(segment.begin(), segment.end(), '/', ' ');
                    std::istringstream ss(segment);
                    if (ss >> v_idx) {
                        f.push_back(v_idx-1);
                    }
                }
                for (int i = 1; i < (int)f.size()-1; i++) {
                    std::vector<int> tri;
                    tri.push_back(f[0]);
                    tri.push_back(f[i]);
                    tri.push_back(f[i+1]);
                    faces_.push_back(tri);
                }
            }
        }
    }

    int nverts() { return (int)verts_.size(); }
    int nfaces() { return (int)faces_.size(); }
    Vec<3> vert(int i) { return verts_[i]; };
    std::vector<int> face(int idx) { return faces_[idx]; }

private:
    std::vector<Vec<3>> verts_;
    std::vector<std::vector<int> > faces_;
};

#endif //TOYENGINE_MODEL_H