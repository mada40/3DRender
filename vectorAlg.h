#pragma once
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

sf::Vector3f normalize(sf::Vector3f& A);
inline float dot(const sf::Vector3f& a, const sf::Vector3f& b) { return a.x * b.x + a.y * b.y + a.z * b.z; }


sf::Vector3f vec_mult(const sf::Vector3f& A, const sf::Vector3f& B);

