#pragma once
#include <vector>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "Camera.h"
#include "OBJ_Loader.h"
class View
{
private:
	sf::Uint8* pixels;
	const sf::Vector3i size;
	float* zbuffer;
public:
	View( sf::Uint8* pixels, const sf::Vector3i& size, float* zbuffer);

	void render(
		const std::vector<objl::Loader>& arrLoaders,
		const std::vector<sf::Image>& arrDiffmap,
		const sf::Vector3f& light, const Matrix& transform);
};

