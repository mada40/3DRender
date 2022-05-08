#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <vector>
#include "OBJ_Loader.h"

#include "Camera.h"
#include "Controller.h"
#include "View.h"
class Scene
{
private:
	Controller controller;
	Camera camera;
	View view;

	const std::vector<objl::Loader> arrLoaders;
	const std::vector<sf::Image> arrDiffmaps;

public:

	Scene(const std::vector<objl::Loader>& arrL,
		const std::vector<sf::Image>& arrD, 
		const Camera& cam, 
		const View& v) 
		: view{ v }, controller{ Controller() }, arrDiffmaps(arrD), arrLoaders(arrL)
	{ camera = cam; }

	
	Scene& operator=(const Scene& other);
	void setCamPos(const sf::Vector3f pos) { camera.position = pos; }
	sf::Vector3f getCamPos() { return camera.position; }

	void update() { controller.moveCamera(camera); }
	void render(const sf::Vector3f light) { view.render(arrLoaders, arrDiffmaps, light, camera.getFinalMatrix()); }

};

