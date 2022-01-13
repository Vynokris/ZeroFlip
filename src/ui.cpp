#include <fstream>

#include "classes.hpp"
#include "Math.hpp"
using namespace Math;



// Constructor that initializes the game window.
Ui::Ui()
{
    // Create the window.
    window.create(sf::VideoMode(1920, 1080), "Zero Flip", sf::Style::Close);
    window.setPosition(sf::Vector2i(-8, 0)); // For some reason x = 0 isn't the left part of the screen, so I move the window to x = -8.
    window.setMouseCursorVisible(false); // To hide the mouse pointer.

    // Set the font for all the text.
    font.loadFromFile("Resources/Art/Font/dpcomic.ttf");

    // Get the player's total score frome the saved file.
    std::ifstream in("Resources/PlayerData/coins.txt");
    std::string line;
    while (std::getline(in, line)) {
        total_score = std::stoi(line);
    }
    in.close();

    // Set the font for the title text.
    for (int i = 0; i < 2; i++)
    {
        title_text[i].setFont(font);
        title_text[i].setCharacterSize(150);
        title_text[i].setFillColor(sf::Color(40, 40, 40));
        title_text[i].setOutlineThickness(3);
        title_text[i].setPosition(720 + i * 300, -25);
        if (i == 0) {
            title_text[i].setOutlineColor(sf::Color(255, 67, 67));
            title_text[i].setString("Zero");
        }
        else {
            title_text[i].setOutlineColor(sf::Color(255, 255, 255));
            title_text[i].setString("Flip");
        }
    }

    // Set the font for the player's total score.
    total_score_text.setFont(font);
    total_score_text.setCharacterSize(130);
    total_score_text.setFillColor(sf::Color(66, 66, 66));
    total_score_text.setOutlineColor(sf::Color(180, 180, 180));
    total_score_text.setOutlineThickness(3);
    total_score_text.setPosition(SCOREBOARD_OFFSET + 60, 310);

    // Set the text for the player's total score.
    if (total_score > 999999) {
        total_score_text.setString("999999");
    }
    // Update the text for the player's total score.
    else {
        int total_score_num_digits = getNumOfDigits(total_score);

        std::string total_score_string = "";
        for (int i = 0; i < 6 - total_score_num_digits; i++) 
            total_score_string += "0";

        total_score_text.setString(total_score_string + std::to_string(total_score));
    }

    // Set the font and text for the player's current score.
    game_score_text.setFont(font);
    game_score_text.setCharacterSize(130);
    game_score_text.setFillColor(sf::Color(66, 66, 66));
    game_score_text.setOutlineColor(sf::Color(180, 180, 180));
    game_score_text.setOutlineThickness(3);
    game_score_text.setString("000001");
    game_score_text.setPosition(SCOREBOARD_OFFSET + 60, 530);

    // Set the font and text for the player's current level.
    game_lv_text.setFont(font);
    game_lv_text.setCharacterSize(120);
    game_lv_text.setFillColor(sf::Color(66, 66, 66));
    game_lv_text.setOutlineColor(sf::Color(180, 180, 180));
    game_lv_text.setOutlineThickness(3);
    game_lv_text.setString("01");
    game_lv_text.setPosition(SCOREBOARD_OFFSET + 275, 680);

    // Set the mouse cursor texture and sprite.
    cursor_texture.loadFromFile("Resources/Art/Cursor/cursor.png");
    cursor_sprite.setTexture(cursor_texture);
    cursor_sprite.setScale(SCALE, SCALE);

    // Load the texture and sprite of the card hover highlight.
    highlight_texture.loadFromFile("Resources/Art/Card/Highlight/highlight.png");
    highlight_sprite.setTexture(highlight_texture);
    highlight_sprite.setScale(SCALE, SCALE);

    // Set the scoreboard texture, sprite and position.
    scoreboard_texture.loadFromFile("Resources/Art/GeneralUI/scoreboard.png");
    scoreboard_sprite.setTexture(scoreboard_texture);
    scoreboard_sprite.setScale(SCALE, SCALE);
    scoreboard_sprite.setPosition(SCOREBOARD_OFFSET, 1080 / 2 - scoreboard_sprite.getGlobalBounds().height / 2);
};



// Processes user input and calls the corresponding functions.
void Ui::input_logic()
{
    if (global_clock.getElapsedTime().asMilliseconds() - last_reset_time >= 600)
    {
        auto mouse_pos = sf::Mouse::getPosition(window);           // Get the mouse position relative to the window.
        auto mouse_world_pos = window.mapPixelToCoords(mouse_pos); // Mouse position translated into world coordinates.
        
        bool is_mouse_on_card = false;
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) 
            {
                // ---------- CARDS INTERACTION ---------- //

                // If the player is hovering over the card.
                if (game_board.board[i][j].sprite.getGlobalBounds().contains(mouse_world_pos)) 
                {
                    // Move the highlight to the card.
                    if ((!game_board.board[i][j].flipped))
                    {
                        highlight_sprite.setPosition(game_board.board[i][j].sprite.getPosition() - sf::Vector2f(SCALE, SCALE));
                        highlight_sprite.setScale(game_board.board[i][j].sprite.getScale());
                        is_mouse_on_card = true;
                    }

                    // If the player left-clicked and the card isn't flipped and the player didn't already flip a card in the last 300 milliseconds.
                    if ((sf::Mouse::isButtonPressed(sf::Mouse::Left) )
                        && (!game_board.board[i][j].flipped)
                        && (global_clock.getElapsedTime().asMilliseconds() - last_flip_time >= 300))
                    {
                        // Flip the card.
                        game_board.board[i][j].flip();
                        last_flip_time = global_clock.getElapsedTime().asMilliseconds();
                    }

                    for (int k = 0; k < 4; k++) {
                        // If the player pressed Numpad 0, 1, 2, or 3 and hasn't already marked a card in the last 300 milliseconds, mark the card accordingly.
                        if ((sf::Keyboard::isKeyPressed(keybindings[k]))
                            && (global_clock.getElapsedTime().asMilliseconds() - last_mark_time >= 300))
                        {
                            // Mark the card as a k.
                            game_board.board[i][j].mark_as(k);
                            last_mark_time = global_clock.getElapsedTime().asMilliseconds();
                        }
                    }
                }


                // ---------- INDICATOR INTERACTION ---------- //

                if (i < 2) 
                {
                    // If the player is hovering over the indicator.
                    if (game_board.indicator[i][j].sprite.getGlobalBounds().contains(mouse_world_pos))
                    {
                        // Make sure that at least one card is not fipped on the indicator's column/row.
                        bool unflipped_card_remaining = false;
                        for (int k = 0; k < 5 && !unflipped_card_remaining; k++)
                        {
                            if (i == 0) {
                                if (!game_board.board[k][j].flipped)
                                    unflipped_card_remaining = true;
                            }
                            else {
                                if (!game_board.board[j][k].flipped)
                                    unflipped_card_remaining = true;
                            }
                        }

                        if (unflipped_card_remaining)
                        {
                            // Move the highlight to the indicator.
                            highlight_sprite.setPosition(game_board.indicator[i][j].sprite.getPosition() - sf::Vector2f(SCALE, SCALE));
                            highlight_sprite.setScale(game_board.indicator[i][j].sprite.getScale());
                            is_mouse_on_card = true;

                            // If the player clicks the indicator, flip the cards accordingly.
                            if ((sf::Mouse::isButtonPressed(sf::Mouse::Left))
                                && game_board.indicator[i][j].zero_num == 0
                                && (global_clock.getElapsedTime().asMilliseconds() - last_flip_time >= 300))
                            {
                                // Flip the cards on the indicator's column/row if they are not already.
                                for (int k = 0; k < 5; k++)
                                {
                                    if (i == 0) {
                                        if (!game_board.board[k][j].flipped)
                                            game_board.board[k][j].flip();
                                    }
                                    else {
                                        if (!game_board.board[j][k].flipped)
                                            game_board.board[j][k].flip();
                                    }
                                }
                                last_flip_time = global_clock.getElapsedTime().asMilliseconds();
                            }

                            // If the player pressed Numpad 0, 1, 2, or 3 and hasn't already marked a card in the last 300 milliseconds, mark the cards accordingly.
                            for (int k = 0; k < 4; k++) 
                            {
                                if ((sf::Keyboard::isKeyPressed(keybindings[k]))
                                    && (global_clock.getElapsedTime().asMilliseconds() - last_mark_time >= 300))
                                {
                                    // Check if all the cards on the column/row are marked as k.
                                    bool all_marked = true;
                                    for (int l = 0; l < 5; l++)
                                    {
                                        if (i == 0) {
                                            if ((!game_board.board[l][j].marked[k]) && (!game_board.board[l][j].flipped))
                                                all_marked = false;
                                        }
                                        else {
                                            if ((!game_board.board[j][l].marked[k]) && (!game_board.board[j][l].flipped))
                                                all_marked = false;
                                        }
                                    }

                                    // Mark/unmark the cards as k on the column/row.
                                    for (int l = 0; l < 5; l++)
                                    {
                                        if (i == 0) {
                                            if (all_marked) {
                                                game_board.board[l][j].mark_as(k);
                                            }
                                            else {
                                                if (!game_board.board[l][j].marked[k]) {
                                                    game_board.board[l][j].mark_as(k);
                                                }
                                            }
                                        }
                                        else {
                                            if (all_marked) {
                                                game_board.board[j][l].mark_as(k);
                                            }
                                            else {
                                                if (!game_board.board[j][l].marked[k]) {
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

                // ---------- PLAYER SCORE UPDATE ---------- //

                if (game_board.board[i][j].sprite.getScale().x <= 0) 
                {
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

                // ---------- CHEATS ---------- //
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) && sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt) && sf::Keyboard::isKeyPressed(sf::Keyboard::Space)
                    && global_clock.getElapsedTime().asMilliseconds() - last_mark_time >= 300)
                {
                    // Marks all the cards' values onto their backs.
                    for (int i = 0; i < 5; i++) {
                        for (int j = 0; j < 5; j++) {
                            game_board.board[i][j].mark_as(game_board.board[i][j].val);
                        }
                    }
                    last_mark_time = global_clock.getElapsedTime().asMilliseconds();
                }
            }
        }

        // Increase or decrease the highlight sprite transparency.
        if (is_mouse_on_card)
        {
            if (highlight_alpha < 255)
                highlight_alpha = clampUnder(highlight_alpha + 10, 255);
        }
        else
        {
            if (highlight_alpha > 0) 
                highlight_alpha = clampAbove(highlight_alpha - 10, 0);
        }

        // ---------- SHOP INTERACTION ---------- //

        if (shop.update(window)) {
            game_board.reloadCardTextures(shop.selected_items[0], shop.selected_items[1], shop.selected_items[2]);
        }

        // If the player presses left click after a game over, reset the game board.
        if (global_clock.getElapsedTime().asMilliseconds() - game_end_time >= 600 && game_board.is_game_over) 
        {
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
    if (game_board.game_won() && global_clock.getElapsedTime().asMilliseconds() - last_reset_time >= 600 and !game_board.is_game_over)
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
    if (game_score == 0 && !game_board.is_game_over)
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

    // Update billy.
    billy.update();
}



// Renders all the game objects onto the game window and handles window events as well as user inputs.
void Ui::render()
{
    // Move the cursor sprite to the mouse position.
    cursor_sprite.setPosition(window.mapPixelToCoords(sf::Mouse::getPosition(window)));

    // Clear the game window.
    window.clear(sf::Color(40, 40, 40));


    // Render the game objects.
    game_board.render(window);

    // Render the card highlight.
    highlight_sprite.setColor(sf::Color(255, 255, 255, highlight_alpha));
    window.draw(highlight_sprite);

    // Render the scoreboard.
    window.draw(scoreboard_sprite);

    // Render the title text.
    for (int i = 0; i < 2; i++)
        window.draw(title_text[i]);

    // Render the total score text.
    window.draw(total_score_text);

    // Render the round score text.
    window.draw(game_score_text);

    // Render the game level text.
    window.draw(game_lv_text);

    // Render the shop.
    shop.render(window);

    // Render billy.
    billy.render(window);

    // Render the mouse cursor.
    window.draw(cursor_sprite);


    // Update the window.
    window.display();
}