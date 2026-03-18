//
// Created by desktop on 26. 3. 17..
//

#include "../include/RenderData.h"

#include <memory>
#include <string>

class Texture;
class Model;
class Material;

using namespace std;

vector<RenderUnit> DataBuilder::build(Model &model, Material &mtl, const string &texture_dir) {
    double min_x = 1e10, max_x = -1e10, min_y = 1e10, max_y = -1e10;
    RenderUnit ru;
    for (int i=0; i<model.nverts(); i++) {
        Vec<3> v = model.vert(i);
        if (v[0] < min_x) min_x = v[0];
        if (v[0] > max_x) max_x = v[0];
        if (v[1] < min_y) min_y = v[1];
        if (v[1] > max_y) max_y = v[1];
    }
    double center_x = (min_x + max_x) * 0.5;
    double center_y = (min_y + max_y) * 0.5;
    ru.p_scale = max(max_x - min_x, max_y - min_y);

    for (auto& [mtl_name, faces] : model.get_material_map()) {
        MaterialData tex = mtl.getMaterial(mtl_name);
        string path;
        if (tex.map_kd.empty()) {
            path = "default";
            cerr << "Missing file -> " << tex.mtlname << endl;
        }else {
            path = "../" + texture_dir + "/" + tex.map_kd;
        }
        ru.tex = make_shared<Texture>(path.c_str());

        for (const auto& tri : faces) {
            for (const auto& fv : tri) {
                Vec<3> v = model.vert(fv.v);
                v[0] -= center_x;
                v[1] -= center_y;

                ru.buffer.push_back(v[0]);
                ru.buffer.push_back(v[1]);
                ru.buffer.push_back(v[2]);
                if (fv.vt >= 0) {
                    Vec<2> uv = model.tex(fv.vt);
                    ru.buffer.push_back(uv[0]);
                    ru.buffer.push_back(uv[1]);
                }else {
                    ru.buffer.push_back(0.);
                    ru.buffer.push_back(0.);
                }
                if (fv.vn >= 0) {
                    Vec<3> vn = model.normal(fv.vn);
                    ru.buffer.push_back(vn[0]);
                    ru.buffer.push_back(vn[1]);
                    ru.buffer.push_back(vn[2]);
                } else {
                    ru.buffer.push_back(0.);
                    ru.buffer.push_back(0.);
                    ru.buffer.push_back(0.);
                }
            }
            ru.f_cnt++;
        }
        res.push_back(ru);
    }
    return res;
}

std::shared_ptr<Texture> TexCache::get_texture(const std::string& path) {
    {
        if (cache.contains(path)) { return cache[path]; }
        auto tex = std::make_shared<Texture>(path.c_str());
        cache[path] = tex;
        return tex;
    }
}