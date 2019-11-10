#include <iostream>
#include <ctime>
#include <fstream>

#include "Matrix.h"
#include "NNBoard.h"
#include "NeuralNetwork.h"

int main()
{
	srand(time(NULL));

	std::ofstream* file = new std::ofstream();
	file->open("data.txt");

	Board* board = new Board();

	for (int i = 0; i < 10000; i++)
	{
		// std::cout << i << ": ";
		int value = board->evaluateBoard();
		std::array<float, 64 * 12> data = board->nnData();

		for (int j = 0; j < 64 * 12; j++)
		{
			*file << data[j] << " ";
		}
		*file << " " << value << "\n";

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
			if (!(board->doRandomMove(i)))
			{
				delete board;
				board = new Board();
			}
		}
	}

	file->close();
	std::cout << "Ended" << std::endl;
	return 0;
}