#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <string>
#include <iostream>
#include <sstream>
#include "MyDraw.h"
#include "Matrix.h"
#include "Camera.h"
#include "OBJ_Loader.h"
#include "vectorAlg.h"
#include "button.h"
#include "View.h"
#include "Scene.h"

int TTT = 0;

const unsigned int W = 1000;
const unsigned int H = 1000;
const int K = 10000;

sf::Vector3f eye(0, 0, 5);
sf::Vector3f DirectionOfView(0, 0, -1);//длина обязательна 1
sf::Vector3f up(0, -1, 0); // не обязательно отогонально DirectionOfView

Camera cam(eye, DirectionOfView, up, 0, 0, std::min(W, H), std::min(W, H), K);
Camera cam2(eye, DirectionOfView, up, 0, 0, std::min(W, H), std::min(W, H), K);


int main()
{
    objl::Loader Loader;
    objl::Loader Loader2;

    bool isOpen = Loader.LoadFile("obj/african_head.obj");
    isOpen &= Loader2.LoadFile("obj/fox.obj");

    if (!isOpen) return 0;
    

    sf::Image diffmap;
    sf::Image diffmap2;
    diffmap.loadFromFile("obj/african_head_diffuse.png");
    diffmap2.loadFromFile("obj/fox_diffuse.png");

    sf::RenderWindow window(sf::VideoMode(W, H), "SFML window");
    //window.setFramerateLimit(60);

    sf::Clock clock;
    float lastTime = 0;

    sf::Uint8* pixels = new sf::Uint8[W * H * 4];
    float* zbuffer = new float[W * H];
    clear(pixels, zbuffer, W * H);

    sf::Font myfont;
    myfont.loadFromFile("obj/Roboto/Roboto-Black.ttf");


    gui::button first("FIRST", myfont, sf::Vector2f(500.f, 200.f), gui::style::save);
    first.setColorNormal(sf::Color::Red);
    first.setColorTextNormal(sf::Color::Green);
    first.setBorderColor(sf::Color::Blue);
    first.setSize(150.f);

    gui::button second("SECOND", myfont, sf::Vector2f(500.f, 500.f), gui::style::save);
    second.setColorNormal(sf::Color::Red);
    second.setColorTextNormal(sf::Color::Green);
    second.setBorderColor(sf::Color::Blue);
    second.setSize(150.f);


    sf::Texture texture;
    texture.create(W, H);
    sf::Sprite sprite(texture); // needed to draw the texture on screen

    sf::Vector3f sun(1, 1, 1);
    normalize(sun);

 
    float T = -3.17 / 2.f * 3.f;

    View view(pixels, sf::Vector3i(W, H, K), zbuffer);


    bool isOpenMenu = true;
    int scene = 0;
    std::vector<Scene> scenes;

    std::vector<objl::Loader> arrL;
    std::vector<sf::Image> arrD;

    arrL.push_back(Loader);
    arrD.push_back(diffmap);
    scenes.push_back(Scene(arrL, arrD, cam, view));

    arrL[0] = (Loader2);
    arrD[0] = (diffmap2);
    scenes.push_back(Scene(arrL, arrD, cam2, view));
    
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

        
        }
        window.clear();
        clear(pixels, zbuffer, W * H);

        first.update(event, window);
        second.update(event, window);

        if (isOpenMenu)
        {
            window.draw(first);
            window.draw(second);

            if (first.getState() == gui::state::clicked)
            {
                isOpenMenu = false;
                scene = 0;
            }

            if (second.getState() == gui::state::clicked)
            {
                isOpenMenu = false;
                scene = 1;
            }  
        }
        else
        {
            T += 0.05f;
            sun.x = cos(T);
            sun.z = sin(T);
            normalize(sun);

            scenes[scene].update();
            scenes[scene].render(sun);

            //drawZbuffer(pixels, W, H, zbuffer, K);

            texture.update(pixels);
            window.draw(sprite);

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            {
                isOpenMenu = true;
            }
        }



        window.display();  

        float currentTime = clock.getElapsedTime().asMilliseconds();
        float fps = 1000.f / (currentTime - lastTime);
        lastTime = currentTime;

        if(TTT % 10 == 0)
            window.setTitle(std::to_string(fps));

    }

    delete[] pixels;
    delete[] zbuffer;

    return EXIT_SUCCESS;
}
