#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <iostream>


#include "classes.h"



// Gives the card a texture and a sprite.
Card::Card() 
{
	texture.loadFromFile("Resources/Art/Card/back.png");
	sprite.setTexture(texture);
	sprite.setScale(scale, scale);
}



// Flips the card over.
void Card::flip()
{
	flipping = 1;
}



// Marks the card as either a 0, a 1, a 2 or a 3.
void Card::mark_as(int num)
{
	if (not flipped) {
		// Invert the mark on the card for the given number.
		marked[num] = !marked[num];

		// If the card was marked, update its texture accordingly.
		if (marked[num]) {
			sf::Image temp;
			temp.loadFromFile("Resources/Art/Card/marked_" + std::to_string(num) + ".png");
			// Paste it onto the card texture in the right place.
			if (num == 0) {
				texture.update(temp, 4, 4);
			}
			else if (num == 1) {
				texture.update(temp, 20, 4);
			}
			else if (num == 2) {
				texture.update(temp, 4, 19);
			}
			else {
				texture.update(temp, 20, 19);
			}
			sprite.setTexture(texture);
		}
		// If the card was unmarked, update its texture accordingly.
		else {
			// Load the texture that will cover the marked number.
			sf::Image temp;
			temp.loadFromFile("Resources/Art/Card/unmark.png");
			// Paste it onto the card texture in the right place.
			if (num == 0) {
				texture.update(temp, 4, 4);
			}
			else if (num == 1) {
				texture.update(temp, 20, 4);
			}
			else if (num == 2) {
				texture.update(temp, 4, 19);
			}
			else {
				texture.update(temp, 20, 19);
			}
			sprite.setTexture(texture);
		}
	}
}



// Renders the card's sprite onto the game window.
void Card::render(sf::RenderWindow& window)
{
	/*
	if (not flipped) {
		// Add the mark number to the card's texture and sprite.
		for (int i = 0; i < 4; i++) {
			// If the user marked the card but the texture has yet to be updated.
			if (marked[i] and not current_texture[i])
			{
				// Load the texture of the marked number.
				sf::Image temp;
				temp.loadFromFile("Resources/Art/Card/marked_" + std::to_string(i) + ".png");
				// Paste it onto the card texture in the right place.
				if (i == 0) {
					texture.update(temp, 4, 4);
				}
				else if (i == 1) {
					texture.update(temp, 20, 4);
				}
				else if (i == 2) {
					texture.update(temp, 4, 19);
				}
				else {
					texture.update(temp, 20, 19);
				}
				sprite.setTexture(texture);
				current_texture[i] = true;
			}

			// If the user unmarked the card but the texture has yet to be updated.
			else if (current_texture[i] and not marked[i])
			{
				// Load the texture that will cover the marked number.
				sf::Image temp;
				temp.loadFromFile("Resources/Art/Card/unmark.png");
				// Paste it onto the card texture in the right place.
				if (i == 0) {
					texture.update(temp, 4, 4);
				}
				else if (i == 1) {
					texture.update(temp, 20, 4);
				}
				else if (i == 2) {
					texture.update(temp, 4, 19);
				}
				else {
					texture.update(temp, 20, 19);
				}
				sprite.setTexture(texture);
				current_texture[i] = true;
			}
		}
	}*/

	// Animate the card flipping.
	if (flipping != 0) {
		// When the card hasn't flipped but is in the process of doing so.
		if (sprite.getScale().x > 0 and flipping == 1) {
			sprite.setScale(sf::Vector2f(float(sprite.getScale().x - 0.08), scale));
			sprite.setPosition(sf::Vector2f(float(sprite.getPosition().x + 0.25 * scale), sprite.getPosition().y));
		}
		// When the card flips.
		else if (sprite.getScale().x <= 0) {
			flipping = 2;
			flipped = !flipped;
			sprite.setScale(sf::Vector2f(float(sprite.getScale().x + 0.08), scale));
			sprite.setPosition(sf::Vector2f(float(sprite.getPosition().x - 0.25 * scale), sprite.getPosition().y));

			// Change the card texture.
			if (flipped) {
				texture.loadFromFile("Resources/Art/Card/front_" + std::to_string(val) + ".png");
			}
			else {
				texture.loadFromFile("Resources/Art/Card/back.png");
			}
			sprite.setTexture(texture);
		}
		// When the card has finished flipping.
		else if (sprite.getScale().x == scale and flipping == 2) {
			flipping = 0;
		}
		// When the card hasn't finished flipping yet but is in the process of doing so.
		else if (sprite.getScale().x > 0 and flipping == 2) {
			sprite.setScale(sf::Vector2f(float(sprite.getScale().x + 0.08), scale));
			sprite.setPosition(sf::Vector2f(float(sprite.getPosition().x - 0.25 * scale), sprite.getPosition().y));
		}
	}

	// Render the card.
	window.draw(sprite);
}



// Resets the card.
void Card::reset(int value) 
{
	val = value;

	for (int i = 0; i < 4; i++) {
		marked[i] = false;
		current_texture[i] = false;
	}

	if (flipped) {
		flip();
	}
	else {
		texture.loadFromFile("Resources/Art/Card/back.png");
		sprite.setTexture(texture);
	}
}