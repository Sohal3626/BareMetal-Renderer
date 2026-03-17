//
// Created by desktop on 26. 3. 17..
//

#ifndef TOYENGINE_DATABUILDER_H
#define TOYENGINE_DATABUILDER_H
#include <memory>
#include <string>

#include "Material.h"
#include "Model.h"
#include "Texture.h"

struct RenderUnit {
    std::string mtlname;
    MaterialData* mat_data;
    std::vector<double> buffer; // 순서는 v v v vt vt vn vn vn
    std::shared_ptr<Texture> tex;
    int f_cnt = 0;
    double p_scale = 1.0;
};

class DataBuilder {
public:
    std::vector<RenderUnit> build(Model &model, Material &mtd_lib, const std::string &texture_dir);
private:
    std::vector<RenderUnit> res;
};

class TexCache {
public:
    std::shared_ptr<Texture> get_texture(const std::string& path);
private:
    std::unordered_map<std::string, std::shared_ptr<Texture>> cache;
};


#endif //TOYENGINE_DATABUILDER_H