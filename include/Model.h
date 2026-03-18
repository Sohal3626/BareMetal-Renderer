//
// Created by desktop on 26. 3. 11..
//

#ifndef TOYENGINE_MODEL_H
#define TOYENGINE_MODEL_H

#include <vector>
#include <string>
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
    explicit Model(const char* filename);
    [[nodiscard]] int nverts() const { return (int)verts_.size(); }
    [[nodiscard]] int nfaces() const { return num_of_faces; }
    [[nodiscard]] int ntex() const { return (int)tex_coords_.size(); }
    Vec<3> vert(int i) { return verts_[i]; };
    Vec<2> tex(int i) { return tex_coords_[i]; }
    Vec<3> normal(int i) const { return {0, 0, 0}; } // 테스트용 더미 함수
    auto get_material_map() { return material_map_; }

private:
    std::vector<Vec<3>> verts_;
    std::vector<Vec<2>> tex_coords_;
    std::unordered_map<std::string, std::vector<std::vector<FaceVertex>>> material_map_;
    int num_of_faces{};
};

#endif //TOYENGINE_MODEL_H