#include <iostream>
#include <fstream>
#include <sstream>
#include <ios>
#include <string>
#include <ctime>

#include "Matrix.h"
#include "NNBoard.h"
#include "NeuralNetwork.h"

void xorProblem();
void train();
void createTrainingData();

int main()
{
	// xorProblem();
	train();
	// createTrainingData();

	// NeuralNetwork<768, 128, 64, 1>* nn = new NeuralNetwork<768, 128, 64, 1>(0.1f);
	// nn->load("nn_.txt");
	// std::cout << "Loaded!!!" << std::endl;
	// nn->save("test.txt");

	return 0;
}

void xorProblem()
{
	NeuralNetwork<2, 2, 2, 1> nn = NeuralNetwork<2, 2, 2, 1>(0.1f);

	for (int i = 0; i < 500000; i++)
	{
		int x = rand() % 2;
		int y = rand() % 2;
		int z = (x + y) % 2;
		std::array<float, 2> input;
		input[0] = x;
		input[1] = y;
		std::shared_ptr<Matrix<1, 1>> target = std::make_shared<Matrix<1, 1>>();
		target->data[0][0] = z;
		auto out = nn.train(input, target);

		if (i > 499900)
			std::cout << out.data[0][0] << ", " << x << ", " << y << ", " << z << std::endl;
	}
}

void train()
{
	srand(time(NULL));

	std::ifstream* file = new std::ifstream("data_1.txt");
	std::string line;

	NeuralNetwork<768, 128, 64, 1>* nn = new NeuralNetwork<768, 128, 64, 1>(0.1f);
	nn->load("nn_.txt");
	
	std::vector<std::array<float, 768>> data;
	std::vector<float> scores;

	if (file->is_open())
	{
		int n = 0;
		while (std::getline(*file, line))
		{
			data.push_back(std::array<float, 768>());
			int i = 0;
			int c = 0;
			while (i < 768)
			{
				if (line[c] != ' ')
				{
					data[n][i] = (float)(line[c] - '0');
					i++;
				}
				c++;
			}
			n++;
			float x = ((float)std::atoi(line.substr(c, line.length() - 1).c_str()) / 10000.0f) + 0.5f;
			scores.push_back(x);
			// std::cout << n << std::endl;
		}
	}

	file->close();
	std::cout << "Data Loaded!" << std::endl;

	for (int j = 0; j < 1000; j++)
	{
		float total_err = 0;
		for (int i = 0; i < data.size(); i++)
		{
			int n = rand() % data.size();
			std::shared_ptr<Matrix<1, 1>> target = std::make_shared<Matrix<1, 1>>();
			target->data[0][0] = scores[n];
			auto result = nn->train(data[n], target);
			float err = 100.f * (result.data[0][0] - target->data[0][0]) / (target->data[0][0]);
			total_err += abs(err);
		}

		int x = rand() % data.size();
		auto arr = data[x];
		auto guess = nn->think(arr);
		std::cout << (10000.f * (guess.data[0][0] - 0.5f)) << " - " << (10000.f * (scores[x] - 0.5f)) << std::endl;
		total_err /= data.size();
		std::cout << j << " %" << total_err << std::endl << std::endl;

		std::stringstream name;
		name << "nn_j" << ".txt";
		nn->save(name.str());
	}

	std::cout << "Trained!!!" << std::endl;
}

void createTrainingData()
{
	srand(time(NULL));

	Board* board = new Board();

	std::ofstream* file = new std::ofstream();
	file->open("data_1.txt");

	for (int i = 0; i < 100000; i++)
	{
		int value = board->evaluateBoard();
		std::array<float, 64 * 12>* data = board->nnData();
		if (i % 1000 == 0)
		{
			std::cout << i << " position completed!\n";
		}

		if (board->state == State::WHITE_MATE || board->state == State::BLACK_MATE || board->state == State::TIE)
		{
			delete board;
			board = new Board();
		}
		else
		{
			bool moved = board->doRandomMove(i);
			if (!moved || i % 2000 == 0)
			{
				// std::cout << "Reseted!" << std::endl;
				*board = Board();
			}
			else
			{
				for (int i = 0; i < data->size(); i++)
				{
					*file << (*data)[i];
					if (i % 12 == 11)
						*file << " ";
				}
				*file << value << "\n";
				// std::cout << board->lastMove << " State: " << (int)board->state << std::endl;
			}
		}
		delete data;
	}

	file->close();
	std::cout << "Ended!" << std::endl;
}

