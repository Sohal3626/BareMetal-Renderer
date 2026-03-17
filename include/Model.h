//
// Created by desktop on 26. 3. 11..
//

#ifndef TOYENGINE_MODEL_H
#define TOYENGINE_MODEL_H

#include <algorithm>
#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <sstream>
#include "Geometry.h"
#include <unordered_map>

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
        std::string curr_mtl;
        std::string line;
        while (getline(in, line)) {
            if (line.empty()) continue;
            std::istringstream iss(line.c_str());
            char trash;
            if (!line.compare(0, 2, "v ")) {
                iss >> trash;
                Vec<3> v;
                iss >> v[0] >> v[1] >> v[2];
                verts_.push_back(v);
            }else if (!line.compare(0, 2, "f ")) {
                std::vector<FaceVertex> fvs;
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
                for (int i = 1; i < fvs.size()-1; i++) {
                    std::vector<FaceVertex> tri;
                    tri.push_back(fvs[0]);
                    tri.push_back(fvs[i]);
                    tri.push_back(fvs[i+1]);
                    material_map_[curr_mtl].push_back(tri);
                    num_of_faces++;
                }
            }else if (!line.compare(0, 3, "vt ")) {
                iss >> trash >> trash;
                Vec<2> uv;
                iss >> uv[0] >> uv[1];
                tex_coords_.push_back(uv);
            }else if (!line.compare(0, 3, "vn ")) {
                //TODO
            }else if (!line.compare(0, 7, "usemtl ")) {
                std::string trash;
                iss >> trash;
                iss >> curr_mtl;
            }
        }
    }

    int nverts() { return (int)verts_.size(); }
    int nfaces() { return num_of_faces; }
    int ntex() { return (int)tex_coords_.size(); }
    Vec<3> vert(int i) { return verts_[i]; };
    Vec<2> tex(int i) { return tex_coords_[i]; }
    Vec<3> normal(int i) { return {0, 0, 0}; } // 테스트용 더미 함수
    auto get_material_map() { return material_map_; }

private:
    std::vector<Vec<3>> verts_;
    std::vector<Vec<2>> tex_coords_;
    std::unordered_map<std::string, std::vector<std::vector<FaceVertex>>> material_map_;
    int num_of_faces;
};

#endif //TOYENGINE_MODEL_H