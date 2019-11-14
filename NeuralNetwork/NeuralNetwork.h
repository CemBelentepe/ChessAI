#pragma once
#include "Matrix.h"
#include <cmath>
#include <array>

template<int input_nodes, int hidden_nodes1, int hidden_nodes2, int output_nodes>
class NeuralNetwork
{
public:
	float learning_rate;

	Matrix<hidden_nodes1, input_nodes> weights_ih;
	Matrix<hidden_nodes2, hidden_nodes1> weights_hh;
	Matrix<output_nodes, hidden_nodes2> weights_ho;

	Matrix<hidden_nodes1, 1> bias_h1;
	Matrix<hidden_nodes2, 1> bias_h2;
	Matrix<output_nodes, 1> bias_o;

	NeuralNetwork(float learning_rate)
		:learning_rate(learning_rate)
	{
		this->weights_ih.randomize();
		this->weights_hh.randomize();
		this->weights_ho.randomize();

		this->bias_h1.randomize();
		this->bias_h2.randomize();
		this->bias_o.randomize();
	}

	template<size_t r, size_t c>
	void activate(std::shared_ptr<Matrix<r, c>>& m)
	{
		for (int i = 0; i < r; i++)
		{
			for (int j = 0; j < c; j++)
			{
				float& x = m->data[i][j];
				x = 1 / (1 + std::exp(-x));
			}
		}
	}

	template<size_t r, size_t c>
	Matrix<r, c> deActivate(const Matrix<r, c>& m)
	{
		Matrix<r, c> result;

		for (int i = 0; i < r; i++)
		{
			for (int j = 0; j < c; j++)
			{
				const float& x = m.data[i][j];
				float& y = result.data[i][j];
				y = x * (1 - x);
			}
		}
		return result;
	}

	Matrix<output_nodes, 1>* think(std::array<float, input_nodes> input_arr)
	{
		auto inputs = Matrix<input_nodes, 1>(input_arr);
		auto hidden1 = Matrix<hidden_nodes1, 1>::multiply(weights_ih, inputs);
		hidden1->addMatrix(bias_h1);
		activate(hidden1);

		auto hidden2 = Matrix<hidden_nodes2, 1>::multiply(weights_hh, *hidden1);
		hidden2->addMatrix(bias_h2);
		activate(hidden2);

		auto outputs = Matrix<output_nodes, 1>::multiply(weights_ho, *hidden2);
		outputs->addMatrix(bias_o);
		activate(outputs);

		return outputs;
	}

	Matrix<output_nodes, 1> train(std::array<float, input_nodes> input_arr, std::shared_ptr<Matrix<output_nodes, 1>> target)
	{
		auto inputs = Matrix<input_nodes, 1>(input_arr);
		auto hidden1 = Matrix<hidden_nodes1, 1>::multiply(weights_ih, inputs);
		hidden1->addMatrix(bias_h1);
		activate(hidden1);

		auto hidden2 = Matrix<hidden_nodes2, 1>::multiply(weights_hh, *hidden1);
		hidden2->addMatrix(bias_h2);
		activate(hidden2);

		auto outputs = Matrix<output_nodes, 1>::multiply(weights_ho, *hidden2);
		outputs->addMatrix(bias_o);
		activate(outputs);

		// Backpropogation
		auto output_err = Matrix<output_nodes, 1>::substract(outputs, target);

		auto who_t = Matrix<hidden_nodes2, output_nodes>::transpose(weights_ho);
		auto hidden_err = Matrix<hidden_nodes2, 1>::multiply(who_t, output_err);

		
		return *outputs;
	}
};
