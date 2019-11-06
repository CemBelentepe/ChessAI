#pragma once
#include "Board.h"

Board* alpha_beta(Board* parent, int player, int32_t alpha, int32_t beta, int depth)
{
	if (depth == 0 || parent->state == State::WHITE_MATE || parent->state == State::BLACK_MATE) return parent;

	std::vector<Board*> childs = parent->getChilds();

	if (parent->currentPlayer == 1)
	{
		int selected = -1;
		int32_t val = INT32_MIN;
		for (int i = 0; i < childs.size(); i++)
		{
			Board* child = alpha_beta(childs[i], childs[i]->currentPlayer, alpha, beta, depth - 1);
			int32_t cval = child->evaluateBoard();
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

		for (int i = 0; i < childs.size(); i++)
		{
			if (i == selected) continue;
			delete childs[i];
		}
		if (selected == -1)
		{
			__debugbreak();
		}
		return childs[selected];
	}
	else
	{
		int selected = -1;
		int32_t val = INT32_MAX;
		for (int i = 0; i < childs.size(); i++)
		{
			Board* child = alpha_beta(childs[i], childs[i]->currentPlayer, alpha, beta, depth - 1);
			int32_t cval = child->evaluateBoard();
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

		for (int i = 0; i < childs.size(); i++)
		{
			if (i == selected) continue;
			delete childs[i];
		}

		if (selected == -1)
		{
			__debugbreak();
		}

		return childs[selected];
	}

	for (int i = 0; i < childs.size(); i++)
	{
		delete childs[i];
	}
}