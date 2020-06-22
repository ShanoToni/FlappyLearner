#pragma once
#include <vector>
#include <random>

class Matrix
{
public:
	Matrix();
	Matrix(int r, int c);
	
private:
	//vars
	int Rows;
	int Cols;
	std::vector<std::vector<float>> Data;
	std::default_random_engine generator;
	std::uniform_real_distribution<float> distribution;

public:
	//get and sets
	std::vector<std::vector<float>> getAsVec() { return Data; }
	//inline void set(std::vector<float> vec) { Data = vec; }
	inline void set(int row, int col, float value) { Data[row][col] = value; }
	void set(std::vector<float> vec);
	inline int getRows() { return Rows; }
	inline int getCols() { return Cols; }

	//functions
	static Matrix fromVector(std::vector<float>vec);
	std::vector<float> toVector();
	void randomize();
	static Matrix transpose(Matrix mat);

	template<typename L>
	void map(L lambda);

	Matrix operator -(Matrix other);
	Matrix operator +(Matrix other);
	Matrix operator *(Matrix other);

	Matrix operator -(float other);
	Matrix operator +(float other);
	Matrix operator *(float other);

	~Matrix();
};

template<typename L>
inline void Matrix::map(L lambda)
{
	for (int i = 0; i < Rows; i++)
	{
		for (int j = 0; j < Cols; j++)
		{
			float value = lambda(getAsVec()[i][j]);
			Data[i][j] = value;
		}
	}
}
