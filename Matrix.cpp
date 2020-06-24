#include "Matrix.h"
#include <iostream>
#include <chrono>

Matrix::Matrix():Matrix(3,3)
{

}

Matrix::Matrix(int r, int c):Rows(r),Cols(c)
{
	for (int i = 0; i < Rows; i++)
	{
		std::vector<float> temp;
		for (int j = 0; j < Cols; j++)
		{
			temp.push_back(0);
		}
		Data.push_back(temp);
	}

	distribution = std::uniform_real_distribution<float>(-1.0f, 1.0f);
}

void Matrix::set(std::vector<float> vec)
{
	for (int i = 0; i < Rows; i++)
	{
		for (int j = 0; j < Cols; j++)
		{
			Data[i][j] = vec[(i+1)*j];
		}
	}
	
}

Matrix Matrix::fromVector(std::vector<float> vec)
{
	Matrix m = Matrix(vec.size(), 1);
	for (int i = 0; i < vec.size(); i++)
	{
		m.set(i, 0, vec[i]);
	}
	return m;
}

std::vector<float> Matrix::toVector()
{
	std::vector<float> vec;

	for (int i = 0; i < Rows; i++)
	{
		for (int j = 0; j < Cols; j++)
		{
			vec.push_back(Data[i][j]);
		}
	}
	return vec;
}

void Matrix::randomize()
{
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);
	for (int i = 0; i < Rows; i++)
	{
		for (int j = 0; j < Cols; j++)
		{
			Data[i][j] = distribution(generator);
		}
	}
}

Matrix Matrix::transpose(Matrix mat)
{
	Matrix transposedMat = Matrix(mat.getCols(), mat.getRows());

	for (int i = 0; i < mat.getRows(); i++)
	{
		for (int j = 0; j < mat.getCols(); j++)
		{
			transposedMat.set(j, i, mat.getAsVec()[i][j]);
		}
	}
	return transposedMat;
}

Matrix Matrix::operator-(Matrix other)
{
	Matrix result(Rows, Cols);
	for (int i = 0; i < Rows; i++)
	{
		for (int j = 0; j < Cols; j++)
		{
			result.set(i, j, getAsVec()[i][j] - other.getAsVec()[i][j]);
		}
	}
	return result;
}

Matrix Matrix::operator+(Matrix other)
{
	Matrix result(Rows, Cols);
	for (int i = 0; i < Rows; i++)
	{
		for (int j = 0; j < Cols; j++)
		{
			result.set(i, j, getAsVec()[i][j] + other.getAsVec()[i][j]);
		}
	}
	return result;
}

Matrix Matrix::operator*(Matrix other)
{
	if (getCols() != other.getRows())
	{
		std::cout << "Nope!" << std::endl;
	}
	Matrix result = Matrix(getRows(), other.getCols());

	for (int i = 0; i < result.getRows(); i++)
	{
		for (int j = 0; j < result.getCols(); j++)
		{
			//set value of result = dot product of rows and cols
			float sum{0};
			for (int k = 0; k < getCols(); k++)
			{
				sum = sum + getAsVec()[i][k] * other.getAsVec()[k][j];
			}

			result.set(i, j, sum);
		}
	}

	return result;
}

Matrix Matrix::operator-(float other)
{
	Matrix result(Rows, Cols);
	for (int i = 0; i < Rows; i++)
	{
		for (int j = 0; j < Cols; j++)
		{
			result.set(i, j, getAsVec()[i][j] - other);
		}
	}
	return result;
}

Matrix Matrix::operator+(float other)
{
	Matrix result(Rows, Cols);
	for (int i = 0; i < Rows; i++)
	{
		for (int j = 0; j < Cols; j++)
		{
			result.set(i, j, getAsVec()[i][j] - other);
		}
	}
	return result;
}

Matrix Matrix::operator*(float other)
{
	Matrix result(Rows, Cols);
	for (int i = 0; i < Rows; i++)
	{
		for (int j = 0; j < Cols; j++)
		{
			result.set(i, j, getAsVec()[i][j] * other);
		}
	}
	return result;
}

Matrix::~Matrix()
{
	Data.clear();
}
