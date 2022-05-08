#include "Controller.h"

void Controller::moveCamera(Camera& cam)
{
    const float speed = cam.speed;
    sf::Vector3f temp = vec_mult(cam.directionOfView, cam.up);
    normalize(temp);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        cam.position.y += speed;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
        cam.position.y -= speed;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        cam.position += cam.directionOfView * speed;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        cam.position -= cam.directionOfView * speed;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        cam.position -= temp * speed;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        cam.position += temp * speed;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        cam.rotationX(0.05f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        cam.rotationX(-0.05f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        cam.rotationY(0.05f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        cam.rotationY(-0.05f);
}
