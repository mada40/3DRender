#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

class Model {
private:
	std::vector<sf::Vector3f> verts_;
	std::vector<std::vector<sf::Vector3i> > faces_; // attention, this Vec3i means vertex/uv/normal
	std::vector<sf::Vector3f> norms_;
	std::vector<sf::Vector2f> uv_;
	void load_texture(std::string filename, const char *suffix, sf::Image &img);
public:
	sf::Image diffusemap_;
	Model(const char *filename);
	~Model();
	int nverts();
	int nfaces();
	sf::Vector3f vert(int i);
	sf::Vector2i uv(int iface, int nvert);
	std::vector<int> face(int idx);
};

#endif //__MODEL_H__
