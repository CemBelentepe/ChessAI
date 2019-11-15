#pragma once
#include "Matrix.h"
#include <cmath>
#include <array>
#include <fstream>
#include <ios>

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
	template<size_t r, size_t c>
	std::shared_ptr<Matrix<r, c>> deActivate(std::shared_ptr<Matrix<r, c>> m)
	{
		std::shared_ptr<Matrix<r, c>> result = std::make_shared <Matrix<r, c>>();

		for (int i = 0; i < r; i++)
		{
			for (int j = 0; j < c; j++)
			{
				const float& x = m->data[i][j];
				float& y = result->data[i][j];
				y = x * (1 - x);
			}
		}

		return result;
	}

	// template<size_t r, size_t c>
	// void activate(std::shared_ptr<Matrix<r, c>>& m)
	// {
	// 	for (int i = 0; i < r; i++)
	// 	{
	// 		for (int j = 0; j < c; j++)
	// 		{
	// 			float& x = m->data[i][j];
	// 			x = x;
	// 		}
	// 	}
	// }
	// 
	// template<size_t r, size_t c>
	// Matrix<r, c> deActivate(const Matrix<r, c>& m)
	// {
	// 	Matrix<r, c> result;
	// 
	// 	for (int i = 0; i < r; i++)
	// 	{
	// 		for (int j = 0; j < c; j++)
	// 		{
	// 			const float& x = m.data[i][j];
	// 			float& y = result.data[i][j];
	// 			y = 1;
	// 		}
	// 	}
	// 	return result;
	// }
	// 
	// template<size_t r, size_t c>
	// std::shared_ptr<Matrix<r, c>> deActivate(std::shared_ptr<Matrix<r, c>> m)
	// {
	// 	std::shared_ptr<Matrix<r, c>> result = std::make_shared <Matrix<r, c>>();
	// 
	// 	for (int i = 0; i < r; i++)
	// 	{
	// 		for (int j = 0; j < c; j++)
	// 		{
	// 			const float& x = m->data[i][j];
	// 			float& y = result->data[i][j];
	// 			y = 1;
	// 		}
	// 	}
	// 
	// 	return result;
	// }

	Matrix<output_nodes, 1> think(std::array<float, input_nodes> input_arr)
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

		return *outputs;
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
		auto output_err = Matrix<output_nodes, 1>::substract(target, outputs);
		auto output_gradients = std::make_shared<Matrix<output_nodes, 1>>();
		*output_gradients = *outputs;
		output_gradients = deActivate(output_gradients);
		output_gradients->elementalMultiply(output_err);
		output_gradients->scalerMultiply(this->learning_rate);
		auto hidden2_T = Matrix<1, hidden_nodes2>::transpose(hidden2);
		auto who_delta = Matrix<output_nodes, hidden_nodes2>::multiply(output_gradients, hidden2_T);
		this->weights_ho.addMatrix(*who_delta);
		this->bias_o.addMatrix(*output_gradients);


		auto who_t = Matrix<hidden_nodes2, output_nodes>::transpose(weights_ho);
		auto hidden2_err = Matrix<hidden_nodes2, 1>::multiply(who_t, output_err);
		auto hidden2_gradients = std::make_shared<Matrix<hidden_nodes2, 1>>();
		*hidden2_gradients = *hidden2;
		hidden2_gradients = deActivate(hidden2_gradients);
		hidden2_gradients->elementalMultiply(hidden2_err);
		hidden2_gradients->scalerMultiply(this->learning_rate);
		auto hidden1_T = Matrix<1, hidden_nodes1>::transpose(hidden1);
		auto whh_delta = Matrix<hidden_nodes2, hidden_nodes1>::multiply(hidden2_gradients, hidden1_T);
		this->weights_hh.addMatrix(*whh_delta);
		this->bias_h2.addMatrix(*hidden2_gradients);


		auto whh_t = Matrix<hidden_nodes1, hidden_nodes2>::transpose(weights_hh);
		auto hidden1_err = Matrix<hidden_nodes1, 1>::multiply(whh_t, hidden2_err);
		auto hidden1_gradients = std::make_shared<Matrix<hidden_nodes1, 1>>();
		*hidden1_gradients = *hidden1;
		hidden1_gradients = deActivate(hidden1_gradients);
		hidden1_gradients->elementalMultiply(hidden1_err);
		hidden1_gradients->scalerMultiply(this->learning_rate);
		auto input_T = Matrix<1, input_nodes>::transpose(inputs);
		auto wih_delta = Matrix<hidden_nodes1, input_nodes>::multiply(hidden1_gradients, input_T);
		this->weights_ih.addMatrix(*wih_delta);
		this->bias_h1.addMatrix(*hidden1_gradients);

		return *outputs;
	}

	void save(std::string name)
	{
		std::ofstream nnFile = std::ofstream();
		nnFile.open(name);
		for (int i = 0; i < hidden_nodes1; i++)
		{
			for (int j = 0; j < input_nodes; j++)
			{
				nnFile << weights_ih.data[i][j] << "\n";
			}
		}

		for (int i = 0; i < hidden_nodes2; i++)
		{
			for (int j = 0; j < hidden_nodes1; j++)
			{
				nnFile << weights_hh.data[i][j] << "\n";
			}
		}

		for (int i = 0; i < output_nodes; i++)
		{
			for (int j = 0; j < hidden_nodes2; j++)
			{
				nnFile << weights_ho.data[i][j] << "\n";
			}
		}

		for (int i = 0; i < hidden_nodes1; i++)
		{
			nnFile << bias_h1.data[i][0] << "\n";
		}

		for (int i = 0; i < hidden_nodes2; i++)
		{
			nnFile << bias_h2.data[i][0] << "\n";
		}

		for (int i = 0; i < output_nodes; i++)
		{
			nnFile << bias_o.data[i][0] << "\n";
		}
		nnFile.close();
	}
};
