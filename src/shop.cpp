#include "classes.hpp"

Shop::Shop()
{
    // Set the shop texture, sprite and position.
    pannel_texture.loadFromFile("Resources/Art/GeneralUI/shop.png");
    pannel_sprite.setTexture(pannel_texture);
    pannel_sprite.setScale(SCALE, SCALE);
    pannel_sprite.setPosition(1920 - SCOREBOARD_OFFSET - pannel_sprite.getGlobalBounds().width, 1080 / 2 - pannel_sprite.getGlobalBounds().height / 2);

    // Set the shop item textures and sprites.
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            std::string texturePath;
            if (i == 0)      texturePath = "Front/front";
            else if (i == 1) texturePath = "Back/back";
            else             texturePath = "Pattern/pattern";

            if (i == 2) {
                items_t[i][j].loadFromFile("Resources/Art/Card/Pattern/patternBg.png");
                sf::Texture patternTexture;
                patternTexture.loadFromFile("Resources/Art/Card/" + texturePath + std::to_string(j) + ".png");
                items_t[i][j].update(patternTexture, 6, 6);
            }
            else {
                items_t[i][j].loadFromFile("Resources/Art/Card/" + texturePath + std::to_string(j) + ".png");
            }
            items_s[i][j].setTexture(items_t[i][j]);
            items_s[i][j].setScale(SCALE / 2, SCALE / 2);

            int item_y_offset = (i == 0 ? 117 : (i == 1 ? 288 : 477));
            items_s[i][j].setPosition(pannel_sprite.getPosition().x + 46 + j * 34 * SCALE/2, 
                                      pannel_sprite.getPosition().y + item_y_offset);
        }
    }

    // Set the three selected item highlights.
    highlight_t.loadFromFile("Resources/Art/Card/Highlight/highlight.png");
    for (int i = 0; i < 3; i++)
    {
        highlight_s[i].setTexture(highlight_t);
        highlight_s[i].setScale(SCALE / 2, SCALE / 2);

        int item_y_offset = (i == 0 ? 117 : (i == 1 ? 288 : 477));
        highlight_s[i].setPosition(pannel_sprite.getPosition().x + 46            - SCALE/2, 
                                   pannel_sprite.getPosition().y + item_y_offset - SCALE/2);
    }

    // TODO: load the player's selected items from file.

    // Move the highlights to the selected items.
    for (int i = 0; i < 3; i++)
    {
        highlight_s[i].setPosition(pannel_sprite.getPosition().x + 46 + selected_items[i] * 34 * SCALE/2 - SCALE/2, 
                                   highlight_s[i].getPosition().y);
    }
}

bool Shop::update(sf::RenderWindow& window)
{
    auto mouse_pos = sf::Mouse::getPosition(window);           // Get the mouse position relative to the window.
    auto mouse_world_pos = window.mapPixelToCoords(mouse_pos); // Mouse position translated into world coordinates.

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            if (items_s[i][j].getGlobalBounds().contains(mouse_world_pos) && sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                // Select the clicked item.
                selected_items[i] = j;

                // Move the highlight to the selected item.
                highlight_s[i].setPosition(pannel_sprite.getPosition().x + 46 + selected_items[i] * 34 * SCALE/2 - SCALE/2, 
                                           highlight_s[i].getPosition().y);

                return true;
            }
        }
    }
    return false;
}

void Shop::render(sf::RenderWindow& window)
{
    // Render the shop.
    window.draw(pannel_sprite);

    // Render the shop items.
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 5; j++)
            window.draw(items_s[i][j]);

    // Render the shop highlights.
    for (int i = 0; i < 3; i++)
    {
        window.draw(highlight_s[i]);
    }
}