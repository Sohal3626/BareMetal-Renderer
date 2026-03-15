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

struct FaceVertex {
    int v;
    int vt;
    int vn;
};

class Model {
public:
    Model(const char* filename) {
        std::ifstream in;
        in.open(filename, std::ifstream::in);
        if (in.fail()) {
            std::cerr << "Missing file -> " << filename << std::endl;
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
                std::vector<FaceVertex> fvs;
                int idx, v_idx, t_idx, n_idx;
                iss >> trash;

                std::string segment;
                while (iss >> segment) {
                    std::replace(segment.begin(), segment.end(), '/', ' ');
                    std::istringstream ss(segment);

                    if (int v = -1; ss >> v) {
                        FaceVertex fv{-1, -1, -1};
                        fv.v = v-1;
                        if (int vt = -1; ss >> vt) fv.vt = vt-1;
                        if (int vn = -1; ss >> vn) fv.vn = vn-1;

                        fvs.push_back(fv);
                    }
                }
                if (!fvs.empty()) {
                    faces_.push_back(fvs);
                }
                for (int i = 1; i < (int)f.size()-1; i++) {
                    std::vector<FaceVertex> tri;
                    tri.push_back(fvs[0]);
                    tri.push_back(fvs[i]);
                    tri.push_back(fvs[i+1]);
                    faces_.push_back(tri);
                }
            }else if (!line.compare(0, 3, "vt ")) {
                iss >> trash >> trash;
                Vec<2> uv;
                iss >> uv[0] >> uv[1];
                tex_coords_.push_back(uv);
            }
        }
    }

    int nverts() { return (int)verts_.size(); }
    int nfaces() { return (int)faces_.size(); }
    int ntex() { return (int)tex_coords_.size(); }
    Vec<3> vert(int i) { return verts_[i]; };
    std::vector<FaceVertex> face(int i) { return faces_[i]; }
    Vec<2> tex(int i) { return tex_coords_[i]; }

private:
    std::vector<Vec<3>> verts_;
    std::vector<std::vector<FaceVertex> > faces_;
    std::vector<Vec<2>> tex_coords_;
};

#endif //TOYENGINE_MODEL_H