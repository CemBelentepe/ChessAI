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
				data[i][j] = 2 * (static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX)) - 1;
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

	void elementalMultiply(std::shared_ptr<Matrix<rows, cols>> other)
	{
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < cols; j++)
			{
				data[i][j] *= other->data[i][j];
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

	static std::shared_ptr<Matrix<rows, cols>> transpose(std::shared_ptr<Matrix<cols, rows>> a)
	{
		std::shared_ptr<Matrix<rows, cols>> result = std::make_shared<Matrix<rows, cols>>();
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < cols; j++)
			{
				result->data[i][j] = a->data[j][i];
			}
		}
		return result;
	}

	static std::shared_ptr<Matrix<rows, cols>> transpose(Matrix<cols, rows>& a)
	{
		std::shared_ptr<Matrix<rows, cols>> result = std::make_shared<Matrix<rows, cols>>();
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < cols; j++)
			{
				result->data[i][j] = a.data[j][i];
			}
		}
		return result;
	}

	static std::shared_ptr<Matrix<rows, cols>> substract(std::shared_ptr<Matrix<rows, cols>> a, std::shared_ptr<Matrix<rows, cols>> b)
	{
		std::shared_ptr<Matrix<rows, cols>> result = std::make_shared<Matrix<rows, cols>>();
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < cols; j++)
			{
				result->data[i][j] = a->data[i][j] - b->data[i][j];
			}
		}
		return result;
	}

	template<size_t p>
	static std::shared_ptr<Matrix<rows, cols>> multiply(const Matrix<rows, p>& a, const Matrix<p, cols>& b)
	{
		std::shared_ptr<Matrix<rows, cols>> result = std::make_shared<Matrix<rows, cols>>();
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < cols; j++)
			{
				float sum = 0;
				for (int k = 0; k < p; k++)
				{
					sum += a.data[i][k] * b.data[k][j];
				}
				result->data[i][j] = sum;
			}
		}
		return result;
	}

	template<size_t p>
	static std::shared_ptr<Matrix<rows, cols>> multiply(std::shared_ptr<Matrix<rows, p>> a, std::shared_ptr<Matrix<p, cols>> b)
	{
		std::shared_ptr<Matrix<rows, cols>> result = std::make_shared<Matrix<rows, cols>>();
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < cols; j++)
			{
				float sum = 0;
				for (int k = 0; k < p; k++)
				{
					sum += a->data[i][k] * b->data[k][j];
				}
				result->data[i][j] = sum;
			}
		}
		return result;
	}
};