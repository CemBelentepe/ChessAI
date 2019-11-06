#include "Board.h"
#include <iostream>

Board::Board()
{
	this->currentPlayer = 0;
	this->selected_x = -1;
	this->selected_y = -1;
	this->selectedMoves = std::vector<Move>();
	this->whiteCastling0 = true;
	this->whiteCastling1 = true;
	this->blackCastling0 = true;
	this->blackCastling1 = true;
	this->state = State::ON_GAME;
}

void Board::displayBoard(sf::RenderWindow& window, sf::Texture* textures)
{
	for (int i = 0; i < 64; i++)
	{
		Type& piece = data[i];
		if (piece != Type::EMPTY)
		{
			sf::Texture& texture = textures[(int)piece];
			sf::Sprite sprite(texture);
			sprite.setOrigin(texture.getSize().x / 2, texture.getSize().y / 2);
			sprite.scale(0.9f, 0.9f);
			sprite.setPosition((i % 8) * 128 + 64, (i / 8) * 128 + 64);
			window.draw(sprite);
		}
	}
	for (Move& move : this->selectedMoves)
	{
		sf::CircleShape shape(16, 32);
		shape.setPosition(move.end_x * 128 + 48, move.end_y * 128 + 48);
		shape.setFillColor(sf::Color::Yellow);
		window.draw(shape);
	}
}

void Board::clickTile(int x, int y)
{
	if (x < 0 && y < 0)
	{
		this->selected_x = -1;
		this->selected_y = -1;
		this->selectedMoves.clear();
	}
	else if (x < 8 && y < 8)
	{
		Type piece = data[8 * y + x];
		if (piece != Type::EMPTY
			&& ((currentPlayer == 0 && (int)piece >= (int)Type::W_PAWN) || currentPlayer == 1 && (int)piece < (int)Type::W_PAWN))
		{
			this->selected_x = x;
			this->selected_y = y;
			this->selectedMoves = this->getMoves(x, y);
		}
		else if (piece == Type::EMPTY || ((currentPlayer == 0 && (int)piece < (int)Type::W_PAWN) || currentPlayer == 1 && (int)piece >= (int)Type::W_PAWN))
		{
			Move move(Type::EMPTY, -1, -1, -1, -1);
			bool flag = false;
			for (int i = 0; i < selectedMoves.size(); i++)
			{
				Move& m = selectedMoves[i];
				if (m.end_x == x && m.end_y == y)
				{
					move = m;
					flag = true;
					break;
				}
			}
			if (flag)
			{
				this->doMove(move, true);
			}
		}
	}
}

void Board::doMove(Move move, bool evaluate_mate)
{
	data[move.start_x + 8 * move.start_y] = Type::EMPTY;
	data[8 * move.end_y + move.end_x] = move.piece;

	if ((move.piece == Type::W_KING || move.piece == Type::B_KING) && move.isCastling)
	{
		if (move.piece == Type::W_KING)
		{
			if (move.end_x == 6)
			{
				data[this->getIndex(7, 7)] = Type::EMPTY;
				data[this->getIndex(5, 7)] = Type::W_ROOK;
			}
			else if (move.end_x == 2)
			{
				data[this->getIndex(0, 7)] = Type::EMPTY;
				data[this->getIndex(3, 7)] = Type::W_ROOK;
			}
			this->whiteCastling0 = false;
			this->whiteCastling1 = false;
		}
		else if (move.piece == Type::B_KING)
		{
			if (move.end_x == 6)
			{
				data[this->getIndex(7, 0)] = Type::EMPTY;
				data[this->getIndex(5, 0)] = Type::B_ROOK;
			}
			else if (move.end_x == 2)
			{
				data[this->getIndex(0, 0)] = Type::EMPTY;
				data[this->getIndex(3, 0)] = Type::B_ROOK;
			}
			this->blackCastling0 = false;
			this->blackCastling1 = false;
		}
	}
	else if (move.piece == Type::W_KING)
	{
		this->whiteCastling0 = false;
		this->whiteCastling1 = false;
	}
	else if (move.piece == Type::B_KING)
	{
		this->blackCastling0 = false;
		this->blackCastling1 = false;
	}
	else if (move.piece == Type::W_ROOK && move.start_y == 7)
	{
		if (whiteCastling0 && move.start_x == 6)
		{
			whiteCastling0 = false;
		}
		else if (whiteCastling1 && move.start_x == 0)
		{
			whiteCastling1 = false;
		}
	}
	else if (move.piece == Type::B_ROOK && move.start_y == 0)
	{
		if (blackCastling0 && move.start_x == 6)
		{
			blackCastling0 = false;
		}
		else if (blackCastling1 && move.start_x == 0)
		{
			blackCastling1 = false;
		}
	}

	this->clickTile(-1, -1);
	if (evaluate_mate) this->state = this->getBoardState();
	else this->state = this->isChecked();
	this->currentPlayer = (this->currentPlayer + 1) % 2;
}

std::vector<Move> Board::getPossibleMoves(int x, int y)
{
	std::vector<Move> moves = std::vector<Move>();
	Type& piece = data[this->getIndex(x, y)];

	if (piece == Type::W_PAWN && y > 0)
	{
		int index = this->getIndex(x, y - 1);
		Type target = data[index];
		if (target == Type::EMPTY)
		{
			if (y == 1)
				moves.push_back(Move(Type::W_QUEEN, x, y, x, y - 1, target));
			else
				moves.push_back(Move(piece, x, y, x, y - 1, target));

			if (y == 6)
			{
				target = data[index - 8];

				if (target == Type::EMPTY)
					moves.push_back(Move(piece, x, y, x, y - 2, target));
			}
		}

		index = this->getIndex(x - 1, y - 1);
		if (index != -1)
		{
			target = data[index];
			if ((int)target < (int)Type::W_PAWN)
			{
				if (y == 1)
					moves.push_back(Move(Type::W_QUEEN, x, y, x - 1, y - 1, target));
				else
					moves.push_back(Move(piece, x, y, x - 1, y - 1, target));
			}
		}

		index = this->getIndex(x + 1, y - 1);
		if (index != -1)
		{
			target = data[index];
			if ((int)target < (int)Type::W_PAWN)
			{
				if (y == 1)
					moves.push_back(Move(Type::W_QUEEN, x, y, x + 1, y - 1, target));
				else
					moves.push_back(Move(piece, x, y, x + 1, y - 1, target));
			}
		}
	}
	else if (piece == Type::B_PAWN && y < 7)
	{
		int index = this->getIndex(x, y + 1);
		Type target = data[index];
		if (target == Type::EMPTY)
		{
			if (y == 6)
				moves.push_back(Move(Type::B_QUEEN, x, y, x, y + 1, target));
			else
				moves.push_back(Move(piece, x, y, x, y + 1, target));

			if (y == 1)
			{
				target = data[index + 8];

				if (target == Type::EMPTY)
					moves.push_back(Move(piece, x, y, x, y + 2, target));
			}
		}

		index = this->getIndex(x - 1, y + 1);
		if (index != -1)
		{
			target = data[index];
			if (((int)target >= (int)Type::W_PAWN) && (target != Type::EMPTY))
			{
				if (y == 6)
					moves.push_back(Move(Type::B_QUEEN, x, y, x - 1, y + 1, target));
				else
					moves.push_back(Move(piece, x, y, x - 1, y + 1, target));
			}
		}

		index = this->getIndex(x + 1, y + 1);
		if (index != -1)
		{
			target = data[index];
			if (((int)target >= (int)Type::W_PAWN) && (target != Type::EMPTY))
			{
				if (y == 6)
					moves.push_back(Move(Type::B_QUEEN, x, y, x + 1, y + 1, target));
				else
					moves.push_back(Move(piece, x, y, x + 1, y + 1, target));
			}
		}
	}
	else if (piece == Type::B_BISHOP || piece == Type::W_BISHOP)
	{
		for (int a = -1; a < 2; a++)
		{
			if (a == 0) continue;

			for (int b = -1; b < 2; b++)
			{
				if (b == 0) continue;

				int i = 1;
				int index = this->getIndex(x + i * a, y + i * b);
				Type target = data[index];
				if (index >= 0)
				{
					while (index >= 0 && (
						target == Type::EMPTY
						|| ((int)target >= (int)Type::W_PAWN && piece == Type::B_BISHOP)
						|| ((int)target < (int)Type::W_PAWN && piece == Type::W_BISHOP)))
					{
						moves.push_back(Move(piece, x, y, x + i * a, y + i * b, target));

						if (target != Type::EMPTY) break;

						i++;
						index = this->getIndex(x + i * a, y + i * b);
						target = data[index];
					}
				}
			}
		}
	}
	else if (piece == Type::B_KING || piece == Type::W_KING)
	{
		for (int a = -1; a < 2; a++)
		{
			for (int b = -1; b < 2; b++)
			{
				if (a == 0 && b == 0) continue;
				int index = this->getIndex(x + a, y + b);
				if (index >= 0)
				{
					Type target = data[index];
					if (target == Type::EMPTY
						|| ((int)target >= (int)Type::W_PAWN && piece == Type::B_KING)
						|| ((int)target < (int)Type::W_PAWN && piece == Type::W_KING))
					{
						moves.push_back(Move(piece, x, y, x + a, y + b, target));
					}
				}
			}
		}
		if (y == 7 && x == 4 && piece == Type::W_KING)
		{
			if (whiteCastling0 && data[getIndex(7, 7)] == Type::W_ROOK && data[getIndex(5, 7)] == Type::EMPTY && data[getIndex(6, 7)] == Type::EMPTY)
				moves.push_back(Move(piece, x, y, 6, 7, Type::EMPTY, true));
			if (whiteCastling1 && data[getIndex(0, 7)] == Type::W_ROOK && data[getIndex(3, 7)] == Type::EMPTY && data[getIndex(2, 7)] == Type::EMPTY && data[getIndex(1, 7)] == Type::EMPTY)
				moves.push_back(Move(piece, x, y, 2, 7, Type::EMPTY, true));
		}
		else if (y == 0 && x == 4 && piece == Type::B_KING)
		{
			if (blackCastling0 && data[getIndex(7, 0)] == Type::B_ROOK && data[getIndex(5, 0)] == Type::EMPTY && data[getIndex(6, 0)] == Type::EMPTY)
				moves.push_back(Move(piece, x, y, 6, 0, Type::EMPTY, true));
			if (blackCastling1 && data[getIndex(0, 0)] == Type::B_ROOK && data[getIndex(3, 0)] == Type::EMPTY && data[getIndex(2, 0)] == Type::EMPTY && data[getIndex(1, 0)] == Type::EMPTY)
				moves.push_back(Move(piece, x, y, 2, 0, Type::EMPTY, true));
		}
	}
	else if (piece == Type::B_KNIGHT || piece == Type::W_KNIGHT)
	{
		for (int i = -2; i < 3; i++)
		{
			for (int j = -2; j < 3; j++)
			{
				int index = this->getIndex(x + i, y + j);
				if (index < 0 || !(i * j == 2 || i * j == -2)) continue;

				Type target = data[index];
				if (target == Type::EMPTY
					|| ((int)target >= (int)Type::W_PAWN && piece == Type::B_KNIGHT)
					|| ((int)target < (int)Type::W_PAWN && piece == Type::W_KNIGHT))
				{
					moves.push_back(Move(piece, x, y, x + i, y + j, target));
				}
			}
		}
	}
	else if (piece == Type::B_ROOK || piece == Type::W_ROOK)
	{
		for (int a = -1; a < 2; a++)
		{
			for (int b = -1; b < 2; b++)
			{
				if (a * b != 0) continue;

				int i = 1;
				int index = this->getIndex(x + i * a, y + i * b);
				Type target = data[index];
				if (index >= 0)
				{
					while (index >= 0 && (
						target == Type::EMPTY
						|| ((int)target >= (int)Type::W_PAWN && piece == Type::B_ROOK)
						|| ((int)target < (int)Type::W_PAWN && piece == Type::W_ROOK)))
					{
						moves.push_back(Move(piece, x, y, x + i * a, y + i * b, target));

						if (target != Type::EMPTY) break;

						i++;
						index = this->getIndex(x + i * a, y + i * b);
						target = data[index];
					}
				}
			}
		}
	}
	else if (piece == Type::B_QUEEN || piece == Type::W_QUEEN)
	{
		for (int a = -1; a < 2; a++)
		{
			for (int b = -1; b < 2; b++)
			{
				if (a == 0 && b == 0) continue;

				int i = 1;
				int index = this->getIndex(x + i * a, y + i * b);
				Type target = data[index];
				if (index >= 0)
				{
					while (index >= 0 && (
						target == Type::EMPTY
						|| ((int)target >= (int)Type::W_PAWN && piece == Type::B_QUEEN)
						|| ((int)target < (int)Type::W_PAWN && piece == Type::W_QUEEN)))
					{
						moves.push_back(Move(piece, x, y, x + i * a, y + i * b, target));

						if (target != Type::EMPTY) break;

						i++;
						index = this->getIndex(x + i * a, y + i * b);
						target = data[index];
					}
				}
			}
		}
	}

	return moves;
}

std::vector<Move> Board::getAllMoves(int player)
{
	std::vector<Move> moves;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			Type& p = data[getIndex(i, j)];
			if ((player == 0 && p < Type::W_PAWN) || (player == 1 && p >= Type::W_PAWN && p < Type::EMPTY))
			{
				std::vector<Move> m = this->getPossibleMoves(i, j);
				moves.insert(moves.end(), m.begin(), m.end());
			}
		}
	}
	return moves;
}

std::vector<Move> Board::getMoves(int x, int y)
{
	std::vector<Move> moves = std::vector<Move>();
	moves = getPossibleMoves(x, y);
	filterMoves(moves);

	return moves;
}

void Board::filterMoves(std::vector<Move>& moves)
{
	for (int i = moves.size() - 1; i >= 0; i--)
	{
		Move& move = moves[i];
		Board* child = new Board(*this);
		child->doMove(move, false);
		State childState = child->state;

		if (childState == State::ON_GAME) continue;

		std::vector<Move> childMoves = child->getAllMoves(child->currentPlayer);
		for (int j = 0; j < childMoves.size(); j++)
		{
			if (child->currentPlayer == 1 && childState == State::BLACK_CHECK)
			{
				moves.erase(moves.begin() + i);
				break;
			}
			else if (child->currentPlayer == 0 && childState == State::WHITE_CHECK)
			{
				moves.erase(moves.begin() + i);
				break;
			}
		}
		delete child;
	}
}

State Board::getBoardState()
{
	State _state = State::ON_GAME;
	std::vector<Move> moves = this->getAllMoves((currentPlayer + 1) % 2);

	for (int i = 0; i < moves.size(); i++)
	{
		Move& move = moves[i];
		if (move.eaten == Type::W_KING)
		{
			_state = State::BLACK_CHECK; // Black checked white
			break;
		}
		else if (move.eaten == Type::B_KING)
		{
			_state = State::WHITE_CHECK; // White checked black
			break;
		}
	}
	if ((_state == State::BLACK_CHECK && currentPlayer == 1) || (_state == State::WHITE_CHECK && currentPlayer == 0))
	{
		std::vector<Move> curr_moves = this->getAllMoves(currentPlayer);
		for (int i = curr_moves.size() - 1; i >= 0; i--)
		{
			Move& move = curr_moves[i];
			Board* child = new Board(*this);
			child->doMove(move, false);
			State childState = child->isChecked();

			if (childState == State::ON_GAME) continue;

			std::vector<Move> childMoves = child->getAllMoves(child->currentPlayer);
			for (int j = 0; j < childMoves.size(); j++)
			{
				if (child->currentPlayer == 0 && childState == State::BLACK_CHECK)
				{
					curr_moves.erase(curr_moves.begin() + i);
					break;
				}
				else if (child->currentPlayer == 1 && childState == State::WHITE_CHECK)
				{
					curr_moves.erase(curr_moves.begin() + i);
					break;
				}
			}
			delete child;
		}
		if (curr_moves.size() == 0)
		{
			if (_state == State::BLACK_CHECK)
			{
				_state = State::WHITE_MATE;
			}
			else
			{
				_state = State::BLACK_MATE;
			}
		}
	}

	return _state;
}

State Board::isChecked()
{
	State _state = State::ON_GAME;
	std::vector<Move> moves = this->getAllMoves(currentPlayer);

	for (int i = 0; i < moves.size(); i++)
	{
		Move& move = moves[i];
		if (move.eaten == Type::W_KING)
		{
			_state = State::BLACK_CHECK; // Black checked white
			break;
		}
		else if (move.eaten == Type::B_KING)
		{
			_state = State::WHITE_CHECK; // White checked black
			break;
		}
	}
	return _state;
}

std::vector<Board*> Board::getChilds()
{
	std::vector<Board*> childs = std::vector<Board*>();
	std::vector<Move> moves = this->getAllMoves(this->currentPlayer);
	for (int i = moves.size() - 1; i >= 0; i--)
	{
		bool add = true;
		Move& move = moves[i];
		Board* child = new Board(*this);
		child->doMove(move, false);
		State childState = child->isChecked();

		std::vector<Move> childMoves = child->getAllMoves(child->currentPlayer);
		for (int j = 0; j < childMoves.size(); j++)
		{
			if (child->currentPlayer == 0 && childState == State::BLACK_CHECK)
			{
				moves.erase(moves.begin() + i);
				add = false;
				break;
			}
			else if (child->currentPlayer == 1 && childState == State::WHITE_CHECK)
			{
				moves.erase(moves.begin() + i);
				add = false;
				break;
			}
		}
		if (add)
		{
			childs.push_back(child);
		}
		else
		{
			delete child;
		}
	}

	for (Board* child : childs)
	{
		child->state = child->getBoardState();
	}

	return childs;
}

int32_t Board::evaluateBoard()
{
	int32_t score = 0;

	if (state == State::WHITE_MATE) return INT32_MIN;
	if (state == State::BLACK_MATE) return INT32_MAX;

	for (int i = 0; i < 64; i++)
	{
		Type piece = data[i];
		switch (piece)
		{
		case Type::B_PAWN:
			score -= 100;
			break;
		case Type::B_ROOK:
			score -= 500;
			break;
		case Type::B_BISHOP:
			score -= 300;
			break;
		case Type::B_KNIGHT:
			score -= 300;
			break;
		case Type::B_QUEEN:
			score -= 900;
			break;
		case Type::W_PAWN:
			score += 100;
			break;
		case Type::W_ROOK:
			score += 500;
			break;
		case Type::W_BISHOP:
			score += 300;
			break;
		case Type::W_KNIGHT:
			score += 300;
			break;
		case Type::W_QUEEN:
			score += 900;
			break;
		}
	}

	score += this->getAllMoves(0).size() - this->getAllMoves(1).size();

	return score;
}
