#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <iostream>
#include <fstream>


#include "classes.h"



// Constructor that initializes the game window.
Ui::Ui()
{
	// Create the window.
	window.create(sf::VideoMode(1920, 1080), "Zero Flip", sf::Style::Close);
	window.setPosition(sf::Vector2i(-8, 0)); // For some reason x = 0 isn't the left part of the screen, so I move the window to x = -8.
	// window.setMouseCursorVisible(false); // To hide the mouse pointer.

	// Set the scoreboard texture, sprite and position.
	scoreboard_texture.loadFromFile("Resources/Art/Indicator/scoreboard.png");
	scoreboard_sprite.setTexture(scoreboard_texture);
	scoreboard_sprite.setScale(game_board.board[0][0].scale, game_board.board[0][0].scale);
	scoreboard_sprite.setPosition(65, 1080 / 2 - scoreboard_sprite.getGlobalBounds().height / 2);

	// Set the font for all the text.
	font.loadFromFile("Resources/Art/Font/dpcomic.ttf");

	// Get the player's total score frome the saved file.
	std::ifstream in("Resources/PlayerData/coins.txt");
	std::string line;
	while (std::getline(in, line)) {
		total_score = std::stoi(line);
	}
	in.close();

	// Set the font for the player's total score.
	total_score_text.setFont(font);
	total_score_text.setCharacterSize(130);
	total_score_text.setFillColor(sf::Color(66, 66, 66));
	total_score_text.setOutlineColor(sf::Color(193, 193, 193));
	total_score_text.setOutlineThickness(3);
	total_score_text.setPosition(125, 310);

	// Set the text for the player's total score.
	if (total_score > 999999) {
		total_score_text.setString("999999");
	}
	else {
		// Get the number of digits in the total score.
		int temp = total_score;
		int total_score_digit_num = 0;
		while (temp > 0) {
			temp /= 10;
			total_score_digit_num++;
		}
		if (total_score == 0) {
			total_score_digit_num = 1;
		}
		// Update the text for the player's total score.
		std::string total_score_string = "";
		for (int i = 0; i < 6 - total_score_digit_num; i++) {
			total_score_string += "0";
		}
		total_score_text.setString(total_score_string + std::to_string(total_score));
	}

	// Set the font and text for the player's current score.
	game_score_text.setFont(font);
	game_score_text.setCharacterSize(130);
	game_score_text.setFillColor(sf::Color(66, 66, 66));
	game_score_text.setOutlineColor(sf::Color(193, 193, 193));
	game_score_text.setOutlineThickness(3);
	game_score_text.setString("000001");
	game_score_text.setPosition(125, 530);

	// Set the font and text for the player's current level.
	game_lv_text.setFont(font);
	game_lv_text.setCharacterSize(120);
	game_lv_text.setFillColor(sf::Color(66, 66, 66));
	game_lv_text.setOutlineColor(sf::Color(193, 193, 193));
	game_lv_text.setOutlineThickness(3);
	game_lv_text.setString("01");
	game_lv_text.setPosition(340, 680);
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

					// Get the number of digits in the total score.
					int temp = game_score;
					int game_score_digit_num = 0;
					while (temp > 0) {
						temp /= 10;
						game_score_digit_num++;
					}
					if (game_score == 0) {
						game_score_digit_num = 1;
					}

					// Update the texts.
					std::string game_score_string = "";
					for (int k = 0; k < 6 - game_score_digit_num; k++) {
						game_score_string += "0";
					}
					game_score_text.setString(game_score_string + std::to_string(game_score));
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

				game_score_text.setString("000001");
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

		// Update the text for the player's total score.
		if (total_score > 999999) {
			total_score_text.setString("999999");
		}
		else {
			// Get the number of digits in the total score.
			int temp = total_score;
			int total_score_digit_num = 0;
			while (temp > 0) {
				temp /= 10;
				total_score_digit_num++;
			}
			// Update the texts.
			std::string total_score_string = "";
			for (int i = 0; i < 6 - total_score_digit_num; i++) {
				total_score_string += "0";
			}
			total_score_text.setString(total_score_string + std::to_string(total_score));
		}
		game_score_text.setString("000001");

		// Reset the game to a higher level.
		if (lv < 99) {
			lv += 1;
		}
		game_board.reset(lv);

		// Update the game level text.
		if (lv < 10) {
			game_lv_text.setString("0" + std::to_string(lv));
		}
		else {
			game_lv_text.setString(std::to_string(lv));
		}
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
		if (lv < 10) {
			game_lv_text.setString("0" + std::to_string(lv));
		}
		else {
			game_lv_text.setString(std::to_string(lv));
		}
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
	//window.clear(sf::Color(41, 164, 107));
	window.clear(sf::Color(40, 40, 40));


	// Render the game objects.
	game_board.render(window);

	// Render the scoreboard.
	window.draw(scoreboard_sprite);

	// Render the total score text.
	window.draw(total_score_text);

	// Render the round score text.
	window.draw(game_score_text);

	// Render the game level text.
	window.draw(game_lv_text);


	// Update the window.
	window.display();
}