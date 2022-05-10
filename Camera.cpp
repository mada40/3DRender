#include "Camera.h"
#include <iostream>

Matrix Camera::lookat() const
{
    sf::Vector3f z = -directionOfView;

    sf::Vector3f x = vec_mult(up, z);
    normalize(x);

    sf::Vector3f y = vec_mult(z, x);
    normalize(y);

    Matrix Minv = Matrix::identity(4);
    Matrix Tr = Matrix::identity(4);
    Minv[0][0] = x.x; Minv[0][1] = x.y; Minv[0][2] = x.z;
    Minv[1][0] = y.x; Minv[1][1] = y.y; Minv[1][2] = y.z;
    Minv[2][0] = z.x; Minv[2][1] = z.y; Minv[2][2] = z.z;

    Tr[0][3] = -(position + directionOfView).x;
    Tr[1][3] = -(position + directionOfView).y;
    Tr[2][3] = -(position + directionOfView).z;

    return Minv * Tr;
}


Matrix Camera::CreateViewport(int x, int y, int _w, int _h, int _d)
{
    Matrix m = Matrix::identity(4);
    m[0][3] = x + _w / 2.f;
    m[1][3] = y + _h / 2.f;
    m[2][3] = _d / 2.f;

    m[0][0] = _w / 2.f;
    m[1][1] = _h / 2.f;
    m[2][2] = _d / 2.f;
    return m;
}



Camera::Camera() : viewport{ Camera::CreateViewport(0, 0, 0, 0, 0) }
{
	position = sf::Vector3f(0.f, 0.f, 0.f); 
	directionOfView = sf::Vector3f(0.f, 0.f, -1.f); 
	up = sf::Vector3f(0.f, -1.f, 0.f); 
    Projection = Matrix::identity(4);
}

Camera::Camera(const sf::Vector3f& pos, const sf::Vector3f& vec, const sf::Vector3f& u, int x, int y, int w, int h, int d)
    :viewport{ Camera::CreateViewport(x, y, w, h, d) }
{
	position = pos; 
	directionOfView = vec; 
	up = u;
    Projection = Matrix::identity(4);
    Projection[3][2] = -1.f;
}

void Camera::rotationX(float angle)
{
    float c = cos(angle);
    float s = sin(angle);
    Matrix rot = Matrix::identity(4);
    rot[0][0] = 1;  rot[0][1] = 0;  rot[0][2] = 0;
    rot[1][0] = 0;  rot[1][1] = c;  rot[1][2] = -s;
    rot[2][0] = 0;  rot[2][1] = s;  rot[2][2] = c;
    directionOfView = (rot * Matrix(directionOfView)).getVector();
    up = (rot * Matrix(up)).getVector();
}

void Camera::rotationY(float angle)
{
    float c = cos(angle);
    float s = sin(angle);
    Matrix rot = Matrix::identity(4);
    rot[0][0] = c;   rot[0][1] = 0;  rot[0][2] = s;
    rot[1][0] = 0;   rot[1][1] = 1;  rot[1][2] = 0;
    rot[2][0] = -s;  rot[2][1] = 0;  rot[2][2] = c;
    directionOfView = (rot * Matrix(directionOfView)).getVector();
}

void Camera::rotationZ(float angle)
{
    float c = cos(angle);
    float s = sin(angle);
    Matrix rot = Matrix::identity(4);
    rot[0][0] = c;  rot[0][1] = -s;  rot[0][2] = 0;
    rot[1][0] = s;  rot[1][1] = c;   rot[1][2] = 0;
    rot[2][0] = 0;  rot[2][1] = 0;   rot[2][2] = 1;
    directionOfView = (rot * Matrix(directionOfView)).getVector();
    up = (rot * Matrix(up)).getVector();
}
