#include <SFML/Graphics.hpp>
#include <sstream>
#include <string>

#include "Board.h"
#include "Minimax.h"
#include "AlphaBeta.h"

const int player = 0;
const bool reverseScreen = false;
int fps_reset = 10;

std::vector<Board> history = std::vector<Board>();

void loadTextures(sf::Texture* textures);
void drawBackground(sf::RenderWindow& window, sf::Texture* t_black, sf::Texture* t_white);
void drawUtils(sf::RenderWindow& window, Board& board, sf::Text& text);
void showFPS(sf::RenderWindow& window, sf::Text& fpsText, sf::Clock& clock, int n, bool update);
void reverseBoard(int n, Board& board);

int main()
{
	sf::RenderWindow window(sf::VideoMode::getFullscreenModes()[0], "Chess!", sf::Style::Default);
	window.setKeyRepeatEnabled(false);
	window.setFramerateLimit(60);
	sf::Clock clock;
	int n = 0;

	// Texture init
	sf::Texture* textures = new sf::Texture[14];
	loadTextures(textures);

	sf::Font font;
	font.loadFromFile("Resources/arial.ttf");

	// Board init
	Board board = Board();
	history.push_back(board);

	sf::Text text = sf::Text();
	text.setFont(font);
	text.setCharacterSize(32);
	text.setPosition(1050, 0);

	sf::Text fpsText = sf::Text();
	fpsText.setFont(font);
	fpsText.setCharacterSize(32);
	fpsText.setPosition(1050, 50);

	// Main Loop
	while (window.isOpen())
	{
		clock.restart();
		sf::Event e;
		while (window.pollEvent(e))
		{
			if (e.type == sf::Event::Closed)
				window.close();
			else if (e.type == sf::Event::KeyPressed)
			{
				if (e.key.code == sf::Keyboard::Escape)
					window.close();
				if (e.key.code == sf::Keyboard::Z)
				{
					if (history.size() > 2)
					{
						board = history[history.size() - 3];
						history.erase(history.end() - 2, history.end());
					}
					else
					{
						board = history[0];
						history.erase(history.begin() + 1, history.end());
					}
				}
			}
		}

		window.clear();

		// RENDER HERE
		drawBackground(window, &textures[12], &textures[13]);

		if (board.currentPlayer == player)
		{
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				bool moved;
				if (reverseScreen)
				{
					moved = board.clickTile(7 - sf::Mouse::getPosition(window).x / 128, 7 - sf::Mouse::getPosition(window).y / 128);
				}
				else
				{
					moved = board.clickTile(sf::Mouse::getPosition(window).x / 128, sf::Mouse::getPosition(window).y / 128);
				}
				if (moved)
				{
					history.push_back(Board(board));
				}
			}
			if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
				board.clickTile(-1, -1);
		}
		else
		{
			board.currentPlayer = player;
			// Board* aiMove = minimax(&board, board.currentPlayer, 3);
			Board* aiMove = alpha_beta(&board, board.currentPlayer, INT32_MIN, INT32_MAX, 3);
			aiMove->currentPlayer = player;
			board = *aiMove;
			history.push_back(board);
		}

		if (board.state > State::BLACK_CHECK)
		{
			board.currentPlayer = -1;
		}

		board.displayBoard(window, textures, reverseScreen);
		drawUtils(window, board, text);
		// showFPS(window, fpsText, clock, n, n==0);

		window.display();
		n = (n + 1) % fps_reset;
	}
}

void drawUtils(sf::RenderWindow& window, Board& board, sf::Text& text)
{
	std::ostringstream stream;
	if (board.currentPlayer == 0)
		stream << "Player: White\n";
	else
		stream << "Player: Black\n";

	switch (board.state)
	{
	case State::ON_GAME:
		stream << "Playing";
		break;
	case State::WHITE_CHECK:
		stream << "White Checked Black";
		break;
	case State::BLACK_CHECK:
		stream << "Black Checked White";
		break;
	case State::WHITE_MATE:
		stream << "Black Won!";
		break;
	case State::BLACK_MATE:
		stream << "White Won!";
		break;
	case State::TIE:
		stream << "Tie!";
		break;
	}
	text.setString(stream.str());
	window.draw(text);
}

void showFPS(sf::RenderWindow& window, sf::Text& fpsText, sf::Clock& clock, int n, bool update)
{
	static float totalTime = 0;
	static float fps = 0;
	auto dt = clock.getElapsedTime().asSeconds();
	totalTime += dt;
	std::ostringstream text;
	text << "Fps: " << fps;
	fpsText.setString(text.str());
	if (update)
	{
		dt /= fps_reset;
		fps = 1.0f / dt;
		totalTime = 0;
	}
	window.draw(fpsText);
}

void reverseBoard(int n, Board& board)
{
	if (history.size() > n)
	{
		board = history[history.size() - n - 1];
		history.erase(history.end() - n, history.end());
	}
	else
	{
		board = history[0];
		history.erase(history.begin() + 1, history.end());
	}
}

void drawBackground(sf::RenderWindow& window, sf::Texture* t_white, sf::Texture* t_black)
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			int y = j;
			if (reverseScreen) y = 7 - y;
			sf::Sprite tile(*t_black);
			tile.setPosition(i * 128, y * 128);
			if ((i + y) % 2 == 0)
			{
				tile.setTexture(*t_white);
			}
			window.draw(tile);
		}
	}
}

void loadTextures(sf::Texture* textures)
{
	textures[0].loadFromFile("Resources/b_pawn.png");
	textures[1].loadFromFile("Resources/b_rook.png");
	textures[2].loadFromFile("Resources/b_bishop.png");
	textures[3].loadFromFile("Resources/b_knight.png");
	textures[4].loadFromFile("Resources/b_queen.png");
	textures[5].loadFromFile("Resources/b_king.png");
	textures[6].loadFromFile("Resources/w_pawn.png");
	textures[7].loadFromFile("Resources/w_rook.png");
	textures[8].loadFromFile("Resources/w_bishop.png");
	textures[9].loadFromFile("Resources/w_knight.png");
	textures[10].loadFromFile("Resources/w_queen.png");
	textures[11].loadFromFile("Resources/w_king.png");
	textures[12].loadFromFile("Resources/square gray light.png");
	textures[13].loadFromFile("Resources/square gray dark.png");
}
