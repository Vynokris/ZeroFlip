#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <iostream>


#include "classes.h"



// Constructor that initializes the game window.
Ui::Ui()
{
	window.create(sf::VideoMode(1920, 1080), "Zero Flip", sf::Style::Close);
	window.setPosition(sf::Vector2i(-8, 0)); // For some reason x = 0 isn't the left part of the screen, so I move the window to x = -8.
	// window.setMouseCursorVisible(false); // To hide the mouse pointer.

	font.loadFromFile("Resources/Art/Font/dpcomic.ttf");

	total_score_text.setFont(font);
	total_score_text.setCharacterSize(70);
	total_score_text.setFillColor(sf::Color::White);
	total_score_text.setString("Total score: 0");
	total_score_text.setPosition(30, 10);

	game_score_text.setFont(font);
	game_score_text.setCharacterSize(70);
	game_score_text.setFillColor(sf::Color::White);
	game_score_text.setString("Score in current game: 1");
	game_score_text.setPosition(30, 80);

	game_lv_text.setFont(font);
	game_lv_text.setCharacterSize(70);
	game_lv_text.setFillColor(sf::Color::White);
	game_lv_text.setString("Level 1");
	game_lv_text.setPosition(1920 - (game_lv_text.getLocalBounds().width + 30), 10);
};



// Processes user input and calls the corresponding functions.
void Ui::input_logic()
{
	if (global_clock.getElapsedTime().asMilliseconds() - last_reset_time >= 600)
	{
		auto mouse_pos = sf::Mouse::getPosition(window); // Get the mouse position relative to the window.
		auto translated_pos = window.mapPixelToCoords(mouse_pos); // Mouse position translated into world coordinates.
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 5; j++) {
				// HANDLE INTERACTION WITH CARDS.
				// If the player is hovering over the card.
				if (game_board.board[i][j].sprite.getGlobalBounds().contains(translated_pos)) 
				{
					// If the player left-clicked and the card isn't flipped and the player didn't already flip a card in the last 300 milliseconds.
					if ((sf::Mouse::isButtonPressed(sf::Mouse::Left) )
						and (not game_board.board[i][j].flipped)
						and (global_clock.getElapsedTime().asMilliseconds() - last_flip_time >= 300))
					{
						// Flip the card.
						game_board.board[i][j].flip();
						last_flip_time = global_clock.getElapsedTime().asMilliseconds();
					}

					for (int k = 0; k < 4; k++) {
						// If the player pressed Numpad 0, 1, 2, or 3 and hasn't already marked a card in the last 300 milliseconds, mark the card accordingly.
						if ((sf::Keyboard::isKeyPressed(keybindings[k]))
							and (global_clock.getElapsedTime().asMilliseconds() - last_mark_time >= 300))
						{
							// Mark the card as a k.
							game_board.board[i][j].mark_as(k);
							last_mark_time = global_clock.getElapsedTime().asMilliseconds();
						}
					}
				}

				// Handle the player's score every time a card is flipped.
				if (game_board.board[i][j].sprite.getScale().x <= 0) {
					game_score *= game_board.board[i][j].val;
					game_score_text.setString("Score in current game: " + std::to_string(game_score));
				}


				// HANDLE INTERACTION WITH INDICATORS.
				if (i < 2) {
					// If the player is hovering over the indicator.
					if (game_board.indicator[i][j].sprite.getGlobalBounds().contains(translated_pos))
					{
						for (int k = 0; k < 4; k++) {
							// If the player pressed Numpad 0, 1, 2, or 3 and hasn't already marked a card in the last 300 milliseconds, mark the card accordingly.
							if ((sf::Keyboard::isKeyPressed(keybindings[k]))
								and (global_clock.getElapsedTime().asMilliseconds() - last_mark_time >= 300))
							{
								// Mark the cards on the indicator's column.
								if (i == 0) {
									// Check if all the cards on the column are marked as k.
									bool all_marked = true;
									for (int l = 0; l < 5; l++) {
										if ((not game_board.board[l][j].marked[k]) and (not game_board.board[l][j].flipped)) {
											all_marked = false;
										}
									}
									for (int l = 0; l < 5; l++) {
										// If all the cards are marked as k, unmark them.
										if (all_marked) {
											game_board.board[l][j].mark_as(k);
										}
										// If they are not all marked as k, make it so they are all marked as k.
										else {
											if (not game_board.board[l][j].marked[k]) {
												game_board.board[l][j].mark_as(k);
											}
										}
									}
								}
								// Mark the cards on the indicator's row.
								else {
									// Check if all the cards on the row are marked as k.
									bool all_marked = true;
									for (int l = 0; l < 5; l++) {
										if ((not game_board.board[j][l].marked[k]) and (not game_board.board[j][l].flipped)) {
											all_marked = false;
										}
									}
									for (int l = 0; l < 5; l++) {
										// If all the cards are marked as k, unmark them.
										if (all_marked) {
											game_board.board[j][l].mark_as(k);
										}
										// If they are not all marked as k, make it so they are all marked as k.
										else {
											if (not game_board.board[j][l].marked[k]) {
												game_board.board[j][l].mark_as(k);
											}
										}
									}
								}
								last_mark_time = global_clock.getElapsedTime().asMilliseconds();
							}
						}
					}
				}
			}
		}

		// If the player presses left click after a game over, reset the game board.
		if (global_clock.getElapsedTime().asMilliseconds() - game_end_time >= 600 and game_board.is_game_over) {
			if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
				last_reset_time = global_clock.getElapsedTime().asMilliseconds();

				game_board.reset(lv);

				game_score = 1;

				game_score_text.setString("Score in current game: " + std::to_string(game_score));
			}
		}
	}
}



// Applies the game rules. Must be run every frame.
void Ui::game_logic()
{
	// Check all the events.
	sf::Event event;
	while (window.pollEvent(event))
	{
		// If the user closed the game window, close the game window.
		if (event.type == sf::Event::Closed) {
			window.close();
		}
	}

	// If the player has flipped all the cards of value 2 or 3.
	if (game_board.game_won() and global_clock.getElapsedTime().asMilliseconds() - last_reset_time >= 600 and not game_board.is_game_over)
	{
		last_reset_time = global_clock.getElapsedTime().asMilliseconds();

		// Add the current game score to the total score and reset the game score.
		total_score += game_score;
		game_score = 1;

		// Update the texts.
		total_score_text.setString("Total score: " + std::to_string(total_score));
		game_score_text.setString("Current game score: " + std::to_string(game_score));

		// Reset the game to a higher level.
		lv += 1;
		game_board.reset(lv);

		// Update the game level text.
		game_lv_text.setString("Level " + std::to_string(lv));
		game_lv_text.setPosition(1920 - (game_lv_text.getLocalBounds().width + 30), 10);
	}


	// If the player has 0 score in the current game, it means he flipped a zero. In that case, end the current game.
	if (game_score == 0 and not game_board.is_game_over) 
	{
		game_end_time = global_clock.getElapsedTime().asMilliseconds();

		// Set the game over variable to true.
		game_board.is_game_over = true;
		
		// Flip all the cards to their front and make the player go down in level by the number of cards he's flipped in this game.
		int flipped_num = game_board.game_over() + 1;

		if (flipped_num < lv) {
			lv = flipped_num;
		}

		// Update the game level text.
		game_lv_text.setString("Level " + std::to_string(lv));
		game_lv_text.setPosition(1920 - (game_lv_text.getLocalBounds().width + 30), 10);
	}

	// If the game is over, make sure all the cards are flipped to the front.
	else if (game_score == 0) {
		game_board.flip_all('F');
	}
}



// Renders all the game objects onto the game window and handles window events as well as user inputs.
void Ui::render()
{
	// Clear the game window.
	window.clear(sf::Color::Black);


	// Render the game objects.
	game_board.render(window);

	// Render the total score text.
	window.draw(total_score_text);

	// Render the round score text.
	window.draw(game_score_text);

	// Render the game level text.
	window.draw(game_lv_text);


	// Update the window.
	window.display();
}