#pragma once
#include "Board.h"
// 
// std::shared_ptr<Board> minimax(std::shared_ptr<Board> parent, int player, int depth)
// {
// 	if (depth == 0 || parent->state == State::WHITE_MATE || parent->state == State::BLACK_MATE) return parent;
// 
// 	std::vector<std::shared_ptr<Board>> childs = parent->getChilds();
// 
// 	if (parent->currentPlayer == 1)
// 	{
// 		int selected = -1;
// 		int32_t val = INT32_MIN;
// 		for (int i = 0; i < childs.size(); i++)
// 		{
// 			std::shared_ptr<Board> child = minimax(childs[i], childs[i]->currentPlayer, depth-1);
// 			int32_t cval = child->evaluateBoard();
// 			if (cval > val)
// 			{
// 				val = cval;
// 				selected = i;
// 			}
// 		}
// 		return childs[selected];
// 	}
// 	else
// 	{
// 		int selected = -1;
// 		int32_t val = INT32_MAX;
// 		for (int i = 0; i < childs.size(); i++)
// 		{
// 			std::shared_ptr<Board> child = minimax(childs[i], childs[i]->currentPlayer, depth - 1);
// 			int32_t cval = child->evaluateBoard();
// 			if (cval < val)
// 			{
// 				val = cval;
// 				selected = i;
// 			}
// 		}
// 		return childs[selected];
// 	}
// 
// }