#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
class Matrix
{
private:
	int w = 0;
	int h = 0;
	float* data = nullptr;
public:
	Matrix() { data = nullptr; w = 0; h = 0; }
	Matrix(const sf::Vector3f& vec) { w = 1; h = 4; data = new float[4]; data[0] = vec.x; data[1] = vec.y; data[2] = vec.z; data[3] = 1.f; }
	Matrix(int n) { w = n; h = n; int size = w*h;  data = new float[size]; for (int i = 0; i < size; i++) data[i] = 0.f; };
	Matrix(int _w, int _h) { w = _w; h = _h; int size = w*h;  data = new float[size]; for (int i = 0; i < size; i++) data[i] = 0.f; };
	Matrix(const Matrix& other);
	~Matrix() { delete[]data; }

	Matrix& operator=(const Matrix& other);
	Matrix operator*(const Matrix& other) const;
	sf::Vector3f getVector() const { return sf::Vector3f(data[0] / data[3], data[1] / data[3], data[2] / data[3]); }

	float* operator[](int i) { return data + i * w; }

	static Matrix identity(int n);
	
};

