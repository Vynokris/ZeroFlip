#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <iostream>


#include "classes.h"



// Default constructor. Gives the right coordinates to each card.
GameBoard::GameBoard() 
{
	// The board is centered in a 1920x1080 screen.
	float x = float(1920 / 2 - (28 * board[0][0].scale + 4 * board[0][0].scale) * 2.5);
	float y = float(1080 / 2 - (28 * board[0][0].scale + 4 * board[0][0].scale) * 2.5);
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			board[i][j].sprite.setPosition(sf::Vector2f(x, y));
			x += 28 * board[0][0].scale + 4 * board[0][0].scale;
		}
		x = float(1920 / 2 - (28 * board[0][0].scale + 4 * board[0][0].scale) * 2.5);
		y += 28 * board[0][0].scale + 4 * board[0][0].scale;
	}

	// Give the right coordinates to each indicator.
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 5; j++) {
			indicator[i][j].set_row_col_index(j);
			indicator[i][j].sprite.setPosition(sf::Vector2f(x, y));
			if (i == 0) {
				x += 28 * board[0][0].scale + 4 * board[0][0].scale;
			}
			else {
				y += 28 * board[0][0].scale + 4 * board[0][0].scale;
			}
		}
		x = float(1920 / 2 + (28 * board[0][0].scale + 4 * board[0][0].scale) * 2.5);
		y = float(1080 / 2 - (28 * board[0][0].scale + 4 * board[0][0].scale) * 2.5);
	}

	// Load the texture and sprite of the lines between the cards.
	between_cards_texture.loadFromFile("Resources/Art/Card/between_cards.png");
	between_cards_sprite.setTexture(between_cards_texture);
	between_cards_sprite.setScale(board[0][0].scale, board[0][0].scale);
	between_cards_sprite.setPosition(565, 145);
}



// Returns true if the only cards that aren't flipped have 1 or 0 of value, false if not.
bool GameBoard::game_won()
{
	bool output = true;
	for (int y = 0; y < 5; y++) {
		for (int x = 0; x < 5; x++) {
			if ((board[y][x].val == 2 or board[y][x].val == 3) and not board[y][x].flipped) {
				output = false;
				x = 5;
				y = 5;
			}
		}
	}
	return output;
}



// Flips over all the cards to the specified side ("B" for back, "F" for front).
void GameBoard::flip_all(char side)
{
	for (int y = 0; y < 5; y++) {
		for (int x = 0; x < 5; x++) {
			if (side == 'F' and not board[y][x].flipped) {
				board[y][x].flip();
			}
			else if (side == 'B' and board[y][x].flipped) {
				board[y][x].flip();
			}
		}
	}
}



// Flips over all the cards to their front side. Returns the number of cards the user had flipped in this game.
int GameBoard::game_over()
{
	int flipped_num = 0;

	for (int y = 0; y < 5; y++) {
		for (int x = 0; x < 5; x++) {
			if (not board[y][x].flipped) {
				board[y][x].flip();
			}
			else if (board[y][x].flipped) {
				flipped_num++;
			}
		}
	}

	return flipped_num;
}



// Renders all the cards on the game board.
void GameBoard::render(sf::RenderWindow& window)
{
	// Render the lines in between the cards.
	window.draw(between_cards_sprite);

	// Render the cards.
	for (int y = 0; y < size; y++) {
		for (int x = 0; x < size; x++) {
			board[y][x].render(window);
		}
	}

	// Render the indicators.
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 5; j++) {
			indicator[i][j].render(window);
		}
	}
}



// Resets the game board. Difficulty is based on the lv argument.
void GameBoard::reset(int lv)
{
	// Reset the cards.
	
	// /*
	// Pick a random number to be the score of the player if he gets all the 2s and 3s.
	int end_score = rand() % (50 * lv) + 500 * lv;

	// Find the number of 2s and 3s needed to make it the player's final score.
	int card_values[4] = { 0, 0, 0, 0 };
	while (true) {
		if (end_score % 3 == 0) {
			card_values[3] += 1;
			end_score /= 3;
		}
		else if (end_score % 2 == 0) {
			card_values[2] += 1;
			end_score /= 2;
		}
		else if (end_score > 1) {
			end_score -= 1;
		}
		else {
			break;
		}
	}

	// Find the number of 1s and 0s to add.
	int current_card_num = card_values[2] + card_values[3];
	if (lv < 4) {
		card_values[0] += (25 - current_card_num) / 3;
	}
	else if (lv < 10) {
		card_values[0] += (25 - current_card_num) / 2;
	}
	else if (lv < 30) {
		card_values[0] += int(round((25 - current_card_num) / 1.5));
	}
	else {
		card_values[0] += int(round((25 - current_card_num) / 1.2));
	}
	current_card_num += card_values[0];
	card_values[1] += 25 - current_card_num;

	std::cout << "Number of 0s: " << card_values[0] << "\nNumber of 1s: " << card_values[1] << "\nNumber of 2s: " << card_values[2] << "\nNUmber of 3s: " << card_values[3] << "\n";

	// Reset the cards.
	int num_of_cards_to_reset = 25;
	for (int y = 0; y < size; y++) {
		for (int x = 0; x < size; x++) {
			int rand_val = rand() % num_of_cards_to_reset;

			int sum = 0;
			for (int i = 0; i < 4; i++) {
				sum += card_values[i];

				if (rand_val <= sum) 
				{
					card_values[i] -= 1;
					board[y][x].reset(i);
					num_of_cards_to_reset -= 1;
					i = 4;
				}
			}
		}
	}
	// */

	/*
	// Array that gives the probability for each possible card value.
	int prob_values[4] = { int(round(sin(float(lv+1) / 5) * 10)),
						   int(round(cos(float(lv) / 5) * 10)),
						   int(round(sin(float(lv+0.5) / 5) * 10)),
						   int(round(sin(float(lv+1) / 5) * 10)) };

	// Calculate the total of those probablilities.
	int prob_total = 0;
	std::cout << "prob_values: ";
	for (int i = 0; i < 4; i++) {
		if (prob_values[i] <= 0) {
			prob_values[i] = 1;
		}
		prob_total += prob_values[i];
		std::cout << prob_values[i] << " ";
	}
	std::cout << "\nprob_total: " << prob_total << "\n";
	// */

	/*
	// Generating card values without rng, only shuffle.
	// Calculate all the probabilities proportionally to the total probability brought down to 25 (number of cards on the board).
	int card_values[4];
	int card_value_sum = 0; // Used to make sure that the values in card_values add up to 25. If not, the missing cards are given the value of 1.
	for (int i = 0; i < 4; i++) {
		card_values[i] = int(prob_values[i] * 25 / prob_total);
		card_value_sum += card_values[i];
	}
	card_values[1] += 25 - card_value_sum; // Give the missing cards a value of 1.

	// Reset the cards according to the card_values array.							TODO: Make the probability of a card's value being 0 if its row or column doesn't have one. rand() % cos((x+y)/10) == 0
	for (int y = 0; y < size; y++) {
		for (int x = 0; x < size; x++) {
			int rand_val = rand() % 4;

			std::cout << "Rand_val: " << int(cos(float(x + y) / 5) * 10) << "\n";
			if (rand() % int(round(cos((x + y) / 5) * 10)) == 0) {
				rand_val = 0;
				std::cout << "Value set to 0.\n";
			}
			else {
				rand_val = rand() % 3 + 1;
			}

			while (card_values[rand_val] == 0) {
				rand_val = rand() % 4;
				std::cout << "Reroll.\n";
			}
			board[y][x].reset(rand_val);
			card_values[rand_val] -= 1;
		}
	}
	// */
	
	/*
	// Generating card values with rng.
	// Make a sorted copy of the probablity array.
	int sorted_prob_values[4];
	std::copy(prob_values, prob_values+4, sorted_prob_values);
	std::sort(sorted_prob_values, sorted_prob_values + 4);

	// Randomize each card's value according to the probability array.
	for (int y = 0; y < size; y++) {
		for (int x = 0; x < size; x++) {
			int rand_val = rand() % (prob_total + 1);

			int sum = 0;
			for (int i = 0; i < 4; i++) {
				sum += sorted_prob_values[i];
				if (rand_val <= sum) {
					std::cout << "Rand value: " << rand_val << " Assigned value: " << std::distance(prob_values, std::find(prob_values, prob_values + 4, sorted_prob_values[i])) << "\n";
					board[y][x].reset(std::distance(prob_values, std::find(prob_values, prob_values + 4, sorted_prob_values[i])));
					i = 4;
				}
			}
		}
	}
	// */
	

	// Reset the indicators.
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 5; j++) {
			// If the indicator is assigned to a column.
			if (i == 0) {
				// Get the sum of the values of the cards on the indicator's column.
				int sum_of_cards = 0;
				for (int k = 0; k < 5; k++) {
					sum_of_cards += board[k][j].val;
				}
				// Get the number of zeros on the indicator's column.
				int num_of_zero = 0;
				for (int k = 0; k < 5; k++) {
					if (board[k][j].val == 0) {
						num_of_zero++;
					}
				}
				// Reset the indicator.
				indicator[i][j].reset(sum_of_cards, num_of_zero);
			}

			// If the indicator is assigned to a row.
			else {
				// Get the sum of the values of the cards on the indicator's row.
				int sum_of_cards = 0;
				for (int k = 0; k < 5; k++) {
					sum_of_cards += board[j][k].val;
				}
				// Get the number of zeros on the indicator's row.
				int num_of_zero = 0;
				for (int k = 0; k < 5; k++) {
					if (board[j][k].val == 0) {
						num_of_zero++;
					}
				}
				// Reset the indicator.
				indicator[i][j].reset(sum_of_cards, num_of_zero);
			}
		}
	}

	// Reset the is_game_over variable.
	is_game_over = false;

	std::cout << lv << "\n";
}