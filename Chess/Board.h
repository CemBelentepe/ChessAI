#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

enum class Type
{
	B_PAWN, B_ROOK, B_BISHOP, B_KNIGHT, B_QUEEN, B_KING,
	W_PAWN, W_ROOK, W_BISHOP, W_KNIGHT, W_QUEEN, W_KING,
	EMPTY
};

enum class State
{
	ON_GAME, WHITE_CHECK, BLACK_CHECK, BLACK_MATE, WHITE_MATE
};

struct Move
{
	Type piece;
	Type eaten;
	int start_x, start_y;
	int end_x, end_y;
	bool isCastling;

	Move(Type piece, int start_x, int start_y, int end_x, int end_y, Type eaten = Type::EMPTY, bool isCastling = false)
		: piece(piece), eaten(eaten), start_x(start_x), start_y(start_y), end_x(end_x), end_y(end_y), isCastling(isCastling)
	{
	}
};

class Board
{
private:
	Type data[64] = {
		Type::B_ROOK,	Type::B_KNIGHT,	Type::B_BISHOP,	Type::B_QUEEN,	Type::B_KING,	Type::B_BISHOP,	Type::B_KNIGHT,	Type::B_ROOK,
		Type::B_PAWN,	Type::B_PAWN,	Type::B_PAWN,	Type::B_PAWN,	Type::B_PAWN,	Type::B_PAWN,	Type::B_PAWN,	Type::B_PAWN,
		Type::EMPTY,	Type::EMPTY,	Type::EMPTY,	Type::EMPTY,	Type::EMPTY,	Type::EMPTY,	Type::EMPTY,	Type::EMPTY,
		Type::EMPTY,	Type::EMPTY,	Type::EMPTY,	Type::EMPTY,	Type::EMPTY,	Type::EMPTY,	Type::EMPTY,	Type::EMPTY,
		Type::EMPTY,	Type::EMPTY,	Type::EMPTY,	Type::EMPTY,	Type::EMPTY,	Type::EMPTY,	Type::EMPTY,	Type::EMPTY,
		Type::EMPTY,	Type::EMPTY,	Type::EMPTY,	Type::EMPTY,	Type::EMPTY,	Type::EMPTY,	Type::EMPTY,	Type::EMPTY,
		Type::W_PAWN,	Type::W_PAWN,	Type::W_PAWN,	Type::W_PAWN,	Type::W_PAWN,	Type::W_PAWN,	Type::W_PAWN,	Type::W_PAWN,
		Type::W_ROOK,	Type::W_KNIGHT,	Type::W_BISHOP,	Type::W_QUEEN,	Type::W_KING,	Type::W_BISHOP,	Type::W_KNIGHT,	Type::W_ROOK
	};
public:
	int currentPlayer;
	int selected_x;
	int selected_y;
	std::vector<Move> selectedMoves;
	bool whiteCastling0, whiteCastling1;
	bool blackCastling0, blackCastling1;
	State state;

	Board();

	void displayBoard(sf::RenderWindow& window, sf::Texture* textures);
	void clickTile(int x, int y);
	void doMove(Move move);

	std::vector<Move> getPossibleMoves(int x, int y);
	std::vector<Move> getAllMoves(int player);
	State getBoardState();
	float evaluateBoard();

	inline int getIndex(int x, int y)
	{
		if (x >= 0 && x < 8 && y >= 0 && y < 8)
			return 8 * y + x;
		else
			return -1;
	}
};
