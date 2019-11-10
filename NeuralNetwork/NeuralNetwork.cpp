#include "NeuralNetwork.h"
#include <cmath>

Matrix NeuralNetwork::think(float* input_arr)
{
	Matrix inputs(input_nodes, 1, input_arr);
	Matrix hidden1 = Matrix::multiply(weights_ih, inputs);
	hidden1.addMatrix(bias_h1);
	activate(hidden1);

	Matrix hidden2 = Matrix::multiply(weights_hh, hidden1);
	hidden2.addMatrix(bias_h2);
	activate(hidden2);

	Matrix outputs = Matrix::multiply(weights_ho, hidden2);
	outputs.addMatrix(bias_o);
	activate(outputs);

	return outputs;
}

Matrix NeuralNetwork::train(float* input_arr, const Matrix& target)
{
	Matrix inputs(input_nodes, 1, input_arr);
	Matrix hidden1 = Matrix::multiply(weights_ih, inputs);
	hidden1.addMatrix(bias_h1);
	activate(hidden1);

	Matrix hidden2 = Matrix::multiply(weights_hh, hidden1);
	hidden2.addMatrix(bias_h2);
	activate(hidden2);

	Matrix outputs = Matrix::multiply(weights_ho, hidden2);
	outputs.addMatrix(bias_o);
	activate(outputs);

	Matrix output_err = Matrix::substract(target, outputs);
	Matrix gradients = deActivate(outputs);
	gradients.elementalMultiply(output_err);
	gradients.scalerMultiply(this->learning_rate);
	Matrix hidden2_T = Matrix::transpose(hidden2);
	Matrix ho_deltas = Matrix::multiply(gradients, hidden2_T);
	this->weights_ho.addMatrix(ho_deltas);
	this->bias_o.addMatrix(gradients);

	Matrix who_T = Matrix::transpose(weights_ho);
	Matrix hidden2_err = Matrix::multiply(who_T, output_err);
	Matrix hidden2_gradiant = deActivate(hidden2);
	hidden2_gradiant.elementalMultiply(hidden2_err);
	hidden2_gradiant.scalerMultiply(this->learning_rate);
	Matrix hidden1_T = Matrix::transpose(hidden1);
	Matrix hh_deltas = Matrix::multiply(hidden2_gradiant, hidden1_T);
	this->weights_hh.addMatrix(hh_deltas);
	this->bias_h2.addMatrix(hidden2_gradiant);


	Matrix whh_T = Matrix::transpose(weights_hh);
	Matrix hidden1_err = Matrix::multiply(whh_T, hidden2_err);
	Matrix hidden1_gradiant = deActivate(hidden1);
	hidden1_gradiant.elementalMultiply(hidden1_err);
	hidden1_gradiant.scalerMultiply(this->learning_rate);
	Matrix input_T = Matrix::transpose(inputs);
	Matrix ih_deltas = Matrix::multiply(hidden1_gradiant, input_T);
	this->weights_ih.addMatrix(ih_deltas);
	this->bias_h1.addMatrix(hidden1_gradiant);

	return Matrix::substract(target, outputs);
}

void NeuralNetwork::activate(Matrix& m)
{
	for (int i = 0; i < m.rows; i++)
	{
		for (int j = 0; j < m.cols; j++)
		{
			float& x = m.data[i][j];
			x = 1 / (1 + std::exp(-x));
		}
	}
}

Matrix NeuralNetwork::deActivate(const Matrix& m)
{
	Matrix result(m.rows, m.cols);
	

	for (int i = 0; i < m.rows; i++)
	{
		for (int j = 0; j < m.cols; j++)
		{
			const float& x = m.data[i][j];
			float& y = m.data[i][j];
			y = x * (1 - x);
		}
	}

	return result;
}
