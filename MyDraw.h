#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>
#define setColor(coor, pix, c){int nnnC = (coor) * 4; pix[nnnC] = (c).r; pix[nnnC + 1] = (c).g; pix[nnnC + 2] = (c).b; pix[nnnC + 3] = (c).a;}

static int T5 = 0;
void F();
void setColor2(unsigned int coor, sf::Uint8*& pixels, const sf::Color& color);

void clear(sf::RenderWindow& window, sf::Uint8*& pixels, float* zbuffer, int size);
void drawLine(const sf::Vector2i& t0, const sf::Vector2i& t1, sf::Uint8*& pixels, int W, int H, const sf::Color& color);
void drawTriangle(
    const sf::Vector3i& t0,
    const sf::Vector3i& t1,
    const sf::Vector3i& t2,
    const sf::Vector2i& tx0,
    const sf::Vector2i& tx1,
    const sf::Vector2i& tx2,
    const sf::Image& diffmap,
    const sf::Image& normal, 
    const sf::Vector3f& light,
    sf::Uint8*& pixels, int W, int H, float* zbuffer);

void drawZbuffer(sf::Uint8* pixels, int W, int H, const float* zbuffer, int d);
