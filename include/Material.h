//
// Created by desktop on 26. 3. 17..
//

#ifndef TOYENGINE_MTL_H
#define TOYENGINE_MTL_H
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>

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
    Material(const char* filename) {
        std::ifstream in;
        in.open(filename);
        if (in.fail()) {
            std::cerr << "Missing file -> " << filename << std::endl;
            return;
        }
        std::map<std::string, MaterialData> map;
        std::string line;
        std::string curr_mtl = "trash";
        MaterialData data;
        while (std::getline(in, line)) {
            if (line.empty()) continue;
            std::istringstream iss(line);
            std::string prefix;
            iss >> prefix;
            if (prefix == "newmtl" || prefix == "newMtl") {
                iss >> curr_mtl;
                map[curr_mtl] = MaterialData();
                map[curr_mtl].mtlname = curr_mtl;
            }else if (!curr_mtl.empty()) {
                if (prefix == "ns") iss >> map[curr_mtl].ns;
                else if (prefix == "ka") {
                    Vec<3> ka;
                    iss >> ka[0];
                    iss >> ka[1];
                    iss >> ka[2];
                    map[curr_mtl].ka = ka;
                }else if (prefix == "kd") {
                    Vec<3> kd;
                    iss >> kd[0];
                    iss >> kd[1];
                    iss >> kd[2];
                    map[curr_mtl].kd = kd;
                }else if (prefix == "ks") {
                    Vec<3> ks;
                    iss >> ks[0];
                    iss >> ks[1];
                    iss >> ks[2];
                    map[curr_mtl].ks = ks;
                }else if (prefix == "ke") {
                    Vec<3> ke;
                    iss >> ke[0];
                    iss >> ke[1];
                    iss >> ke[2];
                    map[curr_mtl].ke = ke;
                }else if (prefix == "ni") iss >> map[curr_mtl].ni;
                else if (prefix == "d") iss >> map[curr_mtl].d;
                else if (prefix == "map_kd") iss >> map[curr_mtl].map_kd;
            }
        }
        mmap = map;
    }
    MaterialData getMaterial(const std::string& mtlname) const {
        auto it = mmap.find(mtlname);
        if (it != mmap.end()) {
            return it->second;
        }
        return {};
    }
private:
    std::map<std::string, MaterialData> mmap;
};


#endif //TOYENGINE_MTL_H