#pragma once
#include <cmath>
#include <iostream>
#include <array>

template <size_t rows, size_t cols>
class Matrix
{
public:
	std::array<std::array<float, cols>, rows> data;

	Matrix()
	{
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < cols; j++)
			{
				data[i][j] = 0;
			}
		}
	}

	Matrix(std::array<float, rows> arr)
	{
		for (int i = 0; i < rows; i++)
		{
			this->data[i][0] = arr[i];
		}
	}

	// ~Matrix()
	// {
	// 	delete[] data;
	// }

	void randomize()
	{
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < cols; j++)
				data[i][j] = 2 * static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX) - 1;
		}
	}

	void elementalMultiply(const Matrix<rows, cols>& other)
	{
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < cols; j++)
			{
				data[i][j] *= other.data[i][j];
			}
		}
	}

	void scalerMultiply(float n)
	{
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < cols; j++)
			{
				data[i][j] *= n;
			}
		}
	}

	void addMatrix(Matrix<rows, cols>& other)
	{
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < cols; j++)
			{
				this->data[i][j] += other.data[i][j];
			}
		}
	}

	void addScaler(float n)
	{
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < cols; j++)
			{
				data[i][j] += n;
			}
		}
	}

	template<size_t c, size_t r>
	static std::shared_ptr<Matrix<c, r>> transpose(std::shared_ptr<Matrix<r, c>> a)
	{
		std::shared_ptr<Matrix<c, r>> result = std::make_shared<Matrix<c, r>>();
		for (int i = 0; i < r; i++)
		{
			for (int j = 0; j < c; j++)
			{
				result->data[i][j] = a->data[j][i];
			}
		}
		return result;
	}


	template<size_t r, size_t c>
	static std::shared_ptr<Matrix<r, c>> substract(std::shared_ptr<Matrix<r, c>> a, std::shared_ptr<Matrix<r, c>> b)
	{
		std::shared_ptr<Matrix<r, c>> result = std::make_shared<Matrix<r, c>>();
		for (int i = 0; i < r; i++)
		{
			for (int j = 0; j < c; j++)
			{
				result->data[i][j] = a.data[i][j] - b.data[i][j];
			}
		}
		return result;
	}


	template<size_t r, size_t c, rsize_t p>
	static std::shared_ptr<Matrix<r, c>> multiply(const Matrix<r, p>& a, const Matrix<p, c>& b)
	{
		std::shared_ptr<Matrix<r, c>> result = std::make_shared<Matrix<r, c>>();
		for (int i = 0; i < r; i++)
		{
			for (int j = 0; j < c; j++)
			{
				float sum = 0;
				for (int k = 0; k < p; k++)
				{
					sum += a.data[i][k] * b.data[k][j];
				}
				result->data[i][j];
			}
		}
		return result;

	}

};