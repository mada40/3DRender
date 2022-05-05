#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "model.h"

Model::Model(const char *filename) : verts_(), faces_(), norms_(), uv_() {
    std::ifstream in;
    in.open (filename, std::ifstream::in);
    if (in.fail()) return;
    std::string line;
    while (!in.eof()) {
        std::getline(in, line);
        std::istringstream iss(line.c_str());
        char trash;
        if (!line.compare(0, 2, "v ")) {
            iss >> trash;
            sf::Vector3f v;
            iss >> v.x >> v.y >> v.z;
            verts_.push_back(v);
        } else if (!line.compare(0, 3, "vn ")) {
            iss >> trash >> trash;
            sf::Vector3f n;
            iss >> n.x >> n.y >> n.z;
            norms_.push_back(n);
        } else if (!line.compare(0, 3, "vt ")) {
            iss >> trash >> trash;
            sf::Vector2f uv;
            iss >> uv.x >> uv.y;
            uv_.push_back(uv);
        }  else if (!line.compare(0, 2, "f ")) {
            std::vector<sf::Vector3i> f;
            sf::Vector3i tmp;
            iss >> trash;
            while (iss >> tmp.x >> trash  >> tmp.y >> trash >> tmp.z) {
                //for (int i=0; i<3; i++) tmp[i]--;// in wavefront obj all indices start at 1, not zero
                tmp.x--;
                tmp.y--;
                tmp.z--;
                f.push_back(tmp);
            }
            faces_.push_back(f);
        }
    }
    std::cerr << "# v# " << verts_.size() << " f# "  << faces_.size() << " vt# " << uv_.size() << " vn# " << norms_.size() << std::endl;
    load_texture(filename, "_diffuse.png", diffusemap_);
}

Model::~Model() {
}

int Model::nverts() {
    return (int)verts_.size();
}

int Model::nfaces() {
    return (int)faces_.size();
}

std::vector<int> Model::face(int idx) {
    std::vector<int> face;
    for (int i=0; i<(int)faces_[idx].size(); i++) face.push_back(faces_[idx][i].x);
    return face;
}

sf::Vector3f Model::vert(int i) {
    return verts_[i];
}

void Model::load_texture(std::string filename, const char *suffix, sf::Image &img) {
    std::string texfile(filename);
    size_t dot = texfile.find_last_of(".");
    if (dot!=std::string::npos) {
        texfile = texfile.substr(0,dot) + std::string(suffix);
        std::cerr << "texture file " << texfile << " loading " << std::endl;
        img.loadFromFile(texfile);
    }
}


sf::Vector2i Model::uv(int iface, int nvert) {
    int idx = faces_[iface][nvert].y;
    return sf::Vector2i(uv_[idx].x*diffusemap_.getSize().x, uv_[idx].y * diffusemap_.getSize().y);
}


