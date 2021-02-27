#pragma once

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <iostream>
#include <string>



class Card
{
public:
	int val = -1;							// Value of the card. Either 0, 1, 2, or 3. It is set to -1 by default.

	bool flipped = false;					// Turns true if the card has been flipped by the player.
	int flipping = 0;						// Turns to 1 if the card is currently in the process of flipping, turns to 2 if the card has flipped but isn't back to its normal scale.

	bool marked[4] = {						// The bools in this array trurn true if the player used the memo to mark the card as the corresponding number.          If marked[0] is true, the card has been marked as a 0 by the player.
		false,
		false,
		false,
		false
	};
	bool current_texture[4] = {				// The bools in this array are set to true if the texture has been applied the corresponding mark number.                If current_texture[0] is true, the card's texture has been updated to show a yellow 0 on it.
		false,
		false,
		false,
		false
	};

	sf::Texture texture;					// The card's texture.
	sf::Sprite sprite;						// The card's sprite.
	float scale = 4.5;						// The scale of the card's sprite.


	// Gives the card a texture and a sprite.
	Card();

	// Flips the card over.
	void flip();

	// Marks the card as either a 0, a 1, a 2 or a 3.
	void mark_as(int num);

	// Renders the card's sprite onto the game window.
	void render(sf::RenderWindow& window);

	// Resets the card.
	void reset(int value);
};



class Indicator
{
public:
	int row_col_index = -1;					// This is the index of the row/column of the indicator.

	int card_sum = -1;						// The sum of the values of the cards that are on the row/column of this indicator.
	int zero_num = -1;						// The number of zeros that are on the row/column of the indicator.

	float scale = 4.5;						// The scale of the indicator's sprite.
	sf::Texture texture;					// The indicator's texture.
	sf::Sprite sprite;						// The indicator's sprite.

	sf::Texture num_texture;				// The texture for each number on the indicator.
	sf::Sprite num_sprite;					// The sprite for each number on the indicator.
	sf::RenderTexture num_rendertexture;	// The rendertexture for each number on the indicator.
	int num_pos[3][2] = {					// The position of position of each number on the indicator.
		{11, 2},
		{19, 2}, 	  
		{19, 15} 
	};



	// Gives the indicator a texture and sprite.
	Indicator();

	// Sets the row/colmns of the indicator.
	void set_row_col_index(int index);

	// Renders the indicator's sprite onto the game window.
	void render(sf::RenderWindow& window);

	// Resets the indicator.
	void reset(int sum_of_cards, int num_of_zeros);
};



class GameBoard
{
public:
	Card board[5][5];						// This is the game board that holds all the flippable cards.
	Indicator indicator[2][5];				// This is the array that holds all the indicators.
	int size = 5;							// This is the width and height of the game board. Meaning it holds 5 rows of 5 cards.

	bool is_game_over = false;				// This turns true if the player flipped a 0.

	sf::Texture between_cards_texture;		// Texture for the lines in between the cards.
	sf::Sprite between_cards_sprite;		// Sprite for the lines in between the cards.


	// Default constructor. Gives the right coordinates to each card.
	GameBoard();

	// Returns true if the only cards that aren't flipped have 1 or 0 of value, false if not.
	bool game_won();

	// Flips over all the cards to the specified side ('B' for back, 'F' for front).
	void flip_all(char side);

	// Flips over all the cards to their front side. Returns the number of cards the user had flipped in this game.
	int game_over();

	// Renders all the cards on the board.
	void render(sf::RenderWindow& window);

	// Resets the game board. Difficulty is based on the lv argument.
	void reset(int lv);
};



class Ui
{
public:
	sf::RenderWindow window;				// Game window onto which all game objects are rendered.
	GameBoard game_board;					// Game board that holds all the flippable cards.
	sf::Clock global_clock;					// Initialize the global internal clock of the game.

	int lv = 1;								// This value determines the difficulty of the game board.
	int total_score = 0;					// Total score of the user, to which the score of each round is added.
	int game_score = 1;						// Player's score in the current game, which gets multiplied by the value of every card flipped.

	sf::Keyboard::Key keybindings[4] = {	// The keys the player can use to mark cards.
		sf::Keyboard::Numpad0,
		sf::Keyboard::Numpad1,
		sf::Keyboard::Numpad2,
		sf::Keyboard::Numpad3,
	};

	int last_flip_time = -100;				// This is the last time a card was flipped.
	int last_mark_time = -100;				// This is the last time the player marked a card with the memo.
	int game_end_time = -100;				// This is the time when the user flips a 0.
	int last_reset_time = -100;				// This is the last time when the game was reset.

	sf::Font font;							// The font used for all text in the ui.
	sf::Text total_score_text;				// Shows the player's total score.
	sf::Text game_score_text;				// Shows the player's score in the current game.
	sf::Text game_lv_text;					// Shows the level of the player's current game.


	// Constructor that initializes the game window.
	Ui();

	// Processes user inputs and calls the corresponding functions.
	void input_logic();

	// Applies the game rules every frame.
	void game_logic();

	// Renders all the game objects onto the game window.
	void render();
};