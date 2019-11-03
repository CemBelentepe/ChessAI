#include <SFML/Graphics.hpp>
#include "Board.h"

void loadTextures(sf::Texture* textures);
void drawBackground(sf::RenderWindow& window, sf::Texture* t_black, sf::Texture* t_white);
void drawUtils(sf::RenderWindow& window, Board& board, sf::Text& text);

int main()
{
	sf::RenderWindow window(sf::VideoMode::getFullscreenModes()[0], "Chess!", sf::Style::Default);
	window.setKeyRepeatEnabled(false);
	window.setFramerateLimit(60);

	// Texture init
	sf::Texture* textures = new sf::Texture[14];
	loadTextures(textures);

	sf::Font font;
	font.loadFromFile("Resources/arial.ttf");

	// Board init
	Board board = Board();

	sf::Text text = sf::Text();
	text.setFont(font);
	text.setCharacterSize(32);
	text.setPosition(1050, 0);

	// Main Loop
	while (window.isOpen())
	{
		sf::Event e;
		while (window.pollEvent(e))
		{
			if (e.type == sf::Event::Closed)
				window.close();
			else if (e.type == sf::Event::KeyPressed)
			{
				if (e.key.code == sf::Keyboard::Escape)
					window.close();
			}
		}

		window.clear();

		// RENDER HERE
		drawBackground(window, &textures[12], &textures[13]);

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			board.clickTile(sf::Mouse::getPosition().x / 128, sf::Mouse::getPosition().y / 128);
		if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
			board.clickTile(-1, -1);

		board.displayBoard(window, textures);
		drawUtils(window, board, text);

		window.display();
	}
}

void drawUtils(sf::RenderWindow& window, Board& board, sf::Text& text)
{
	switch (board.state)
	{
	case State::ON_GAME:
		if (board.currentPlayer == 0)
			text.setString("Player: White");
		else
			text.setString("Player: Black");
		break;
	case State::WHITE_CHECK:
		text.setString("White Checked Black");
		break;
	case State::BLACK_CHECK:
		text.setString("Black Checked White");
		break;
	case State::WHITE_MATE:
		text.setString("Black won!");
		break;
	case State::BLACK_MATE:
		text.setString("White won!");
		break;
	}
	window.draw(text);
}

void drawBackground(sf::RenderWindow& window, sf::Texture* t_white, sf::Texture* t_black)
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			sf::Sprite tile(*t_black);
			tile.setPosition(i * 128, j * 128);
			if ((i + j) % 2 == 0)
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
