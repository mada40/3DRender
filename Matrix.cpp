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

sf::Vector3f Matrix::operator*(const sf::Vector3f& v) const
{
	float x = data[0] * v.x + data[1] * v.y + data[2] * v.z + data[3];
	float y = data[4] * v.x + data[5] * v.y + data[6] * v.z + data[7];
	float z = data[8] * v.x + data[9] * v.y + data[10] * v.z + data[11];
	return sf::Vector3f(x, y, z);
}


float Matrix::det3x3(const Matrix& matr)
{
	return matr[0][0] * matr[1][1] * matr[2][2] - matr[0][0] * matr[1][2] * matr[2][1]
		- matr[0][1] * matr[1][0] * matr[2][2] + matr[0][1] * matr[1][2] * matr[2][0]
		+ matr[0][2] * matr[1][0] * matr[2][1] - matr[0][2] * matr[1][1] * matr[2][0];
}

Matrix Matrix::inverseTransposeMatrix4x4(const Matrix& m)
{
	Matrix ans(4);
	int coeff[2] = { 1,-1 };
	float det = 0.f;//разложение по последнему столбцу
	for (int y = 0; y < 4; y++)
	{
		float minor;
		for (int x = 0; x < 4; x++)
		{
			minor = m.getMinorForMatrix4x4(y, x);
			ans.data[4*y + x] = coeff[(x + y) % 2] * minor;
		}
		det += m.data[y * 4 + 3] * coeff[(y + 1) % 2] * minor;
	}

	return ans / det;

}

float Matrix::getMinorForMatrix4x4(int _y, int _x) const
{	
	Matrix ans(3);
	int matY = 0;
	for (int y = 0; y < 4; y++)
	{
		if (y == _y) continue;

		int matX = 0;
		for (int x = 0; x < 4; x++)
		{
			if (x == _x) continue;

			ans.data[matY * 3 + matX] = data[4 * y + x];
			matX++;
		}
		matY++;
	}
	return det3x3(ans);
}

Matrix Matrix::identity(int n)
{
	Matrix E(n);
	for (int i = 0; i < n; i++) 
		for (int j = 0; j < n; j++) 
			E[i][j] = (i == j ? 1.f : 0.f);

	return E;
}
