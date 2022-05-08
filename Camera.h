#pragma once
#include "Matrix.h"
#include "vectorAlg.h"
class Camera
{
private:

	Matrix viewport;
	Matrix Projection = Matrix::identity(4);

	Matrix CreateViewport(int x, int y, int _w, int _h, int _d);
	Matrix lookat() const;
	void projection();

public:
	sf::Vector3f position;
	sf::Vector3f directionOfView;
	sf::Vector3f up;
	float speed = 0.1f;
	Camera();
	Camera(const sf::Vector3f& pos, const sf::Vector3f& vec, const sf::Vector3f& u, int x, int y, int w, int h, int d);
	
	Matrix getFinalMatrix() { projection();  return viewport * Projection * lookat(); }
	void rotationX(float angle);
	void rotationY(float angle);
	void rotationZ(float angle);
};

