#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <iostream>

#include "classes.hpp"



// Gives the indicator a texture and sprite.
Indicator::Indicator() 
{
    // Load the indicator's texture and sprite.
    texture.loadFromFile("Resources/Art/Indicator/column_row_0.png");
    sprite.setTexture(texture);
    sprite.setScale(SCALE, SCALE);
    
    // Set the number render texture size.
    num_rendertexture.create(6, 8);
}



// Sets the row/column index of the indicator
void Indicator::set_row_col_index(int index)
{
    row_col_index = index;

    texture.loadFromFile("Resources/Art/Indicator/column_row_" + std::to_string(row_col_index) + ".png");

    sprite.setTexture(texture);
}



// Renders the indicator on the game window.
void Indicator::render(sf::RenderWindow& window)
{
    window.draw(sprite);
}



// Resets the indicator.
void Indicator::reset(int sum_of_cards, int num_of_zeros)
{
    // Reset the number of zeros and the sum of the values of the cards on the line of the indicator.
    card_sum = sum_of_cards;
    zero_num = num_of_zeros;

    // Reset the texture.
    texture.loadFromFile("Resources/Art/Indicator/column_row_" + std::to_string(row_col_index) + ".png");

    for (int i = 0; i < 3; i++)
    {
        // Clear the temp_render_texture.
        num_rendertexture.clear();

        // Colored background.
        num_texture.loadFromFile("Resources/Art/Indicator/background_" + std::to_string(row_col_index) + ".png");
        num_sprite.setTexture(num_texture);
        num_rendertexture.draw(num_sprite);

        // Number.
        if (i == 0) {
            num_texture.loadFromFile("Resources/Art/Indicator/" + std::to_string(int(card_sum / 10)) + ".png");
        }
        else if (i == 2) {
            num_texture.loadFromFile("Resources/Art/Indicator/" + std::to_string(zero_num) + ".png");
        }
        else {
            num_texture.loadFromFile("Resources/Art/Indicator/" + std::to_string(card_sum % 10) + ".png");
        }

        num_sprite.setTexture(num_texture);
        num_rendertexture.draw(num_sprite);

        // Update the temp_render_texture.
        num_rendertexture.display();

        // Load the resulting texture onto the indicator's texture.
        texture.update(num_rendertexture.getTexture(), num_pos[i][0], num_pos[i][1]);
    }

    // Load the new texture onto the sprite.
    sprite.setTexture(texture);
}