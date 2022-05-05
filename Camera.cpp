#include "Camera.h"
#include <iostream>

void Camera::normal(sf::Vector3f& A) {
    double l = sqrt(A.x * A.x + A.y * A.y + A.z * A.z);
    A.x /= l;
    A.y /= l;
    A.z /= l;
}

sf::Vector3f Camera::vec_mult(const sf::Vector3f& A, const sf::Vector3f& B) {
    sf::Vector3f norm;
    norm.x = (A.y * B.z - A.z * B.y);
    norm.y = (A.z * B.x - A.x * B.z);
    norm.z = (A.x * B.y - A.y * B.x);
    return norm;
}

void Camera::lookat()
{
    sf::Vector3f z = -directionOfView;

    sf::Vector3f x = vec_mult(up, z);
    normal(x);

    sf::Vector3f y = vec_mult(z, x);
    normal(y);

    Matrix Minv = Matrix::identity(4);
    Matrix Tr = Matrix::identity(4);
    Minv[0][0] = x.x; Minv[0][1] = x.y; Minv[0][2] = x.z;
    Minv[1][0] = y.x; Minv[1][1] = y.y; Minv[1][2] = y.z;
    Minv[2][0] = z.x; Minv[2][1] = z.y; Minv[2][2] = z.z;

    Tr[0][3] = -(position + directionOfView).x;
    Tr[1][3] = -(position + directionOfView).y;
    Tr[2][3] = -(position + directionOfView).z;

    ModelView = Minv * Tr;
}

void Camera::projection()
{
    Projection[3][2] = -1.f / sqrt(directionOfView.x * directionOfView.x + directionOfView.y * directionOfView.y + directionOfView.z * directionOfView.z);
}

Camera::Camera()
{
	position = sf::Vector3f(0.f, 0.f, 0.f); 
	directionOfView = sf::Vector3f(0.f, 0.f, -1.f); 
	up = sf::Vector3f(0.f, -1.f, 0.f); 
	viewport = Matrix::CreateViewport(0, 0, 0, 0, 0);
    Projection = Matrix::identity(4);
}

Camera::Camera(const sf::Vector3f& pos, const sf::Vector3f& vec, const sf::Vector3f& u, int x, int y, int w, int h, int d)
{
	position = pos; 
	directionOfView = vec; 
	up = u;
	viewport = Matrix::CreateViewport(x, y, w, h, d);
    Projection = Matrix::identity(4);
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
