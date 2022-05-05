#pragma once
#include "Matrix.h"
class Camera
{
private:

	Matrix viewport;
	Matrix Projection = Matrix::identity(4);
	Matrix ModelView;
public:
	sf::Vector3f position;
	sf::Vector3f directionOfView;
	sf::Vector3f up;
	void normal(sf::Vector3f& A);
	sf::Vector3f vec_mult(const sf::Vector3f& A, const sf::Vector3f& B);
	void lookat();
	void projection();
	Camera();
	Camera(const sf::Vector3f& pos, const sf::Vector3f& vec, const sf::Vector3f& u, int x, int y, int w, int h, int d);
	
	Matrix getFinalMatrix() { projection(); lookat();  return viewport * Projection * ModelView; }
	void rotationX(float angle);
	void rotationY(float angle);
	void rotationZ(float angle);
};

