#include "Scene.h"

Scene& Scene::operator=(const Scene& other)
{
    if (this == &other) return *this;
    camera = other.camera;
    controller = other.controller;

    return *this;
}
