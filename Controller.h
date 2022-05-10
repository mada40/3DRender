#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "Camera.h"

class Controller
{
public:
	void moveCamera(Camera& cam, float deltatime);
};

