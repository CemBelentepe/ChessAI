#include <iostream>
#include <fstream>
#include <ios>
#include <string>
#include <ctime>

#include "Matrix.h"
#include "NNBoard.h"
#include "NeuralNetwork.h"

void train();
void createTrainingData();

int main()
{
	train();
	// createTrainingData();
	return 0;
}

void train()
{
	srand(time(NULL));
	std::ifstream* file = new std::ifstream("data.txt");
	std::string line;

	NeuralNetwork<768, 128, 64, 1>* nn = new NeuralNetwork<768, 128, 64, 1>(0.1f);
	std::vector<std::array<float, 768>> data;
	std::vector<int> scores;

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
			scores.push_back(std::atoi(line.substr(c, line.length() - 1).c_str()));
			// std::cout << n << std::endl;
		}
	}

	std::cout << "Data Loaded!" << std::endl;
	for (int i = 0; i < data.size(); i++)
	{
		std::shared_ptr<Matrix<1, 1>> target = std::make_shared<Matrix<1,1>>();
		target->data[0][0] = scores[i];
		auto result = nn->train(data[i], target);
		// std::cout << result.data[0][0] << ", " << target->data[0][0] << std::endl;
	}
	std::cout << "Trained!!!" << std::endl;
}

void createTrainingData()
{
	srand(time(NULL));

	Board* board = new Board();

	std::ofstream* file = new std::ofstream();
	file->open("data.txt");

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
			if (!moved || i%2000 == 0)
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

