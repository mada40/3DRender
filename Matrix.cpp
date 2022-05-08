#include "Matrix.h"

inline Matrix::Matrix(const Matrix& other)
{
	w = other.w;
	h = other.h;
	int size = w * h;
	data = new float[size];
	for (int i = 0; i < size; i++)
		data[i] = other.data[i];
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
	Matrix ans(other.w, h);
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


Matrix Matrix::identity(int n)
{
	Matrix E(n);
	for (int i = 0; i < n; i++) 
		for (int j = 0; j < n; j++) 
			E[i][j] = (i == j ? 1.f : 0.f);

	return E;
}
