#include "Controller.h"

void Controller::moveCamera(Camera& cam, float deltatime)
{
    const float speed = cam.speed * deltatime;
    const float angle = 0.005f * deltatime;
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
        cam.rotationX(angle);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        cam.rotationX(-angle);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        cam.rotationY(angle);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        cam.rotationY(-angle);
}
