//
// Created by desktop on 26. 3. 17..
//

#ifndef TOYENGINE_MTL_H
#define TOYENGINE_MTL_H
#include <map>
#include <string>

#include "Geometry.h"

struct MaterialData {
    std::string mtlname;
    std::string map_kd;
    double ns = 0.0;
    Vec<3> ka = {0, 0, 0};
    Vec<3> kd = {0, 0, 0};
    Vec<3> ks = {0, 0, 0};
    Vec<3> ke = {0, 0, 0};
    double ni = 0.0;
    double d = 0.0;
};

class Material {
public:
    explicit Material(const char* filename);
    [[nodiscard]] MaterialData getMaterial(const std::string& mtlname) const;
private:
    std::map<std::string, MaterialData> mmap;
};


#endif //TOYENGINE_MTL_H