#include "Matrix.h"

inline Matrix::Matrix(const Matrix& other)
{
	w = other.w;
	h = other.h;
	int size = w * h;
	data = new float[size];
	for (int i = 0; i < size; i++)
	{
		data[i] = other.data[i];
	}
}

Matrix& Matrix::operator=(const Matrix& other)
{
	if (this == &other) return *this;

	delete[] data;
	w = other.w;
	h = other.h;
	int size = w * h;
	data = new float[size];
	for (int i = 0; i < size; i++)
		data[i] = other.data[i];

	return *this;
}

Matrix Matrix::operator*(const Matrix& other) const
{
	Matrix ans(h, other.w);
	int size = h* other.w;

	for (int i = 0; i < size; i++)
		for (int j = 0; j < w; j++)
		{
			int y = i / other.w;
			int x = i % other.w;
			ans.data[i] += data[y * w + j] * other.data[j * other.w + x];
		}

	return ans;
}

Matrix Matrix::CreateViewport(int x, int y, int _w, int _h, int _d)
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

Matrix Matrix::identity(int n)
{
	Matrix E(n);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			E[i][j] = (i == j ? 1.f : 0.f);
		}
	}
	return E;
}
