#pragma once
#include "Board.h"
NeuralNetwork<768, 128, 64, 1>* nn;
int alphaBetas;
std::shared_ptr<Board> alpha_beta(std::shared_ptr<Board> parent, int32_t alpha, int32_t beta, int depth)
{
	alphaBetas++;
	if (depth == 0 || parent->state == State::WHITE_MATE || parent->state == State::BLACK_MATE || parent->state == State::TIE) return parent;

	std::vector<std::shared_ptr<Board>> childs = parent->getChilds();

	if (parent->currentPlayer == 1)
	{
		int selected = -1;
		int32_t val = INT32_MIN;
		for (int i = 0; i < childs.size(); i++)
		{
			std::shared_ptr<Board> child = alpha_beta(childs[i], alpha, beta, depth - 1);
			int32_t cval = child->evaluateBoard(nn);
			if (cval >= val)
			{
				val = cval;
				selected = i;
			}
			if (val > alpha) alpha = val;
			if (alpha >= beta)
			{
				break;
			}
		}

		return childs[selected];
	}
	else
	{
		int selected = -1;
		int32_t val = INT32_MAX;
		for (int i = 0; i < childs.size(); i++)
		{
			std::shared_ptr<Board> child = alpha_beta(childs[i], alpha, beta, depth - 1);
			int32_t cval = child->evaluateBoard(nn);
			if (cval <= val)
			{
				val = cval;
				selected = i;
			}
			if (val < beta) beta = val;
			if (alpha >= beta)
			{
				break;
			}
		}
		return childs[selected];
	}
}