#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "model.h"
#include <SFML/System.hpp>
#include <string>
#include <iostream>
#include <sstream>
#include "MyDraw.h"
#include "Matrix.h"
#include "Camera.h"
#include "OBJ_Loader.h"
using namespace std;

void normal(sf::Vector3f& A) {
    double l = sqrt(A.x * A.x + A.y * A.y + A.z * A.z);
    A.x /= l;
    A.y /= l;
    A.z /= l;
}
inline float dot(const sf::Vector3f& a, const sf::Vector3f& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

sf::Vector3f vec_mult(const sf::Vector3f& A, const sf::Vector3f& B) {
    sf::Vector3f norm;
    norm.x = (A.y * B.z - A.z * B.y);
    norm.y = (A.z * B.x - A.x * B.z);
    norm.z = (A.x * B.y - A.y * B.x);
    return norm;
}


int TTT = 0;


sf::Vector3f eye(0, 0, 3);
sf::Vector3f DirectionOfView(0, 0, -1);//длина обязательна 1
sf::Vector3f up(0, -1, 0); // не обязательно отогонально DirectionOfView

const unsigned int W = 600;
const unsigned int H = 600;
const int K = 1000;
Camera cam(eye, DirectionOfView, up, 0, 0, min(W, H), min(W, H), K);

float speed = 0.1f;
void management()
{
    sf::Vector3f temp = vec_mult(cam.directionOfView, cam.up);
    normal(temp);

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


char arr[2048 * 2048];
int main()
{
    objl::Loader Loader;
    cout << Loader.LoadFile("obj/cottage/cottage_obj.obj") << "--------" << endl;

    sf::Image diffmap;
    diffmap.loadFromFile("obj/cottage/cottage_textures/cottage_diffuse.png");

    sf::Image normalMap;
    normalMap.loadFromFile("obj/cottage/cottage_textures/cottage_normal.png");


    sf::RenderWindow window(sf::VideoMode(W, H), "SFML window");
    //window.setFramerateLimit(60);

    sf::Clock clock;
    float lastTime = 0;

    sf::Uint8* pixels = new sf::Uint8[W * H * 4];
    float* zbuffer = new float[W * H];
    clear(window, pixels, zbuffer, W * H);




    sf::Texture texture;
    texture.create(W, H);
    sf::Sprite sprite(texture); // needed to draw the texture on screen

    sf::Vector3f sun(1, 1, 1);
    normal(sun);



    float T = 0;
    float a = 0;
    while (window.isOpen())
    {
        T += 0.05f;
        sun.x = cos(T);
        sun.z = sin(T);
        //sun.z = 0;
        normal(sun);
        clear(window, pixels, zbuffer, W * H);
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        management();

        
        Matrix A = cam.getFinalMatrix();

        for (objl::Mesh mesh : Loader.LoadedMeshes)
        {
            for (int i = 0; i < mesh.Indices.size(); i += 3)
            {
                sf::Vector3i sc[3];
                sf::Vector2i tx[3];
                for (int j = 0; j < 3; j++)
                {
                    auto t = mesh.Vertices[mesh.Indices[i + j]];
                    tx[j] = sf::Vector2i(t.TextureCoordinate.X * 2048, 2048 - t.TextureCoordinate.Y * 2048);
                    sf::Vector3f temp = sf::Vector3f(t.Position.X / 5.f, t.Position.Y / 5.f, t.Position.Z  /5.f);
                    temp = (A * Matrix(temp)).getVector();
                    sc[j] = sf::Vector3i(temp.x, temp.y, temp.z);
                }
                

                if (sc[0].z <= 2 * K && sc[0].z > 0.f &&
                    sc[1].z <= 2 * K && sc[1].z > 0.f &&
                    sc[2].z <= 2 * K && sc[2].z > 0.f)
                {
                    drawTriangle(
                        sc[0], sc[1], sc[2],
                        tx[0], tx[1], tx[2],
                        diffmap, normalMap, sun, pixels, W, H, zbuffer);
                }

            }


        }
     


        //drawZbuffer(pixels, W, H, zbuffer, K);

        texture.update(pixels);
        window.draw(sprite);
        window.display();

        


        float currentTime = clock.getElapsedTime().asMilliseconds();
        float fps = 1000.f / (currentTime - lastTime);
        lastTime = currentTime;

        if(TTT % 10 == 0)
            window.setTitle(to_string(fps));

    }

    delete[] pixels;
    delete[] zbuffer;

    return EXIT_SUCCESS;
}
