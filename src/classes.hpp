#pragma once

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <utility>

#define SCALE 4.5

#define SCOREBOARD_OFFSET 20
#define GAME_BOARD_OFFSET 535



// Draw the given texture onto the given rendertexture at the given coordinates.
static inline void rendertextureDraw(sf::RenderTexture& rendertexture, sf::Texture& texture, int x, int y)
{
    sf::Sprite sprite;
    sprite.setTexture(texture);
    sprite.setPosition(x, y);
    rendertexture.draw(sprite);
    rendertexture.display();
}
// Draw the given texture onto the given rendertexture at the given coordinates.
static inline void rendertextureDraw(sf::RenderTexture& rendertexture, std::string texturePath, int x, int y)
{
    sf::Texture texture;
    texture.loadFromFile(texturePath);
    sf::Sprite sprite;
    sprite.setTexture(texture);
    sprite.setPosition(x, y);
    rendertexture.draw(sprite);
    rendertexture.display();
}



enum class GameEvents
{
    START_GAME,
    END_GAME,
    FLIP_CARD,
    MARK_CARD,
    BUY_COSMETIC,
    CHANGE_COSMETIC,
    CLICKED_BILLY,
};

enum class BillyEmotions
{
    NORMAL,
    HAPPY,
    SAD,
    CRYING,
    DISAPPOINTED,
    SMALL_FACE,
};

class Billy
{
public:
    // Billy's current textures.
    int current_texture = 0;
    int current_text    = -1;

    // The number of frames until billy goes back to his normal emotion.
    int emotion_cooldown = 0;

    // Billy's current emotion.
    BillyEmotions emotion = BillyEmotions::NORMAL;

    // List of the most recent actions performed by the player.
    std::vector<std::pair<GameEvents, std::vector<int>>> gameEvents;

    // Billy's texture and sprite.
    sf::RenderTexture rendertexture;
    sf::Sprite        sprite;

    // The texture and sprite for billy's text.
    sf::Texture textTexture;
    sf::Sprite  textSprite;


    // Loads billy's texture and creates his sprite.
    Billy();

    // Adds a game event to billy's list so he can track what the player does.
    void addEvent(GameEvents eventType, std::vector<int> data);

    // Reload billy's texture.
    void reloadTexture();

    // Update billy.
    void update(sf::RenderWindow& window);

    // Draw billy on the window.
    void render(sf::RenderWindow& window);
};



class Card
{
public:
    int val = -1;         // Value of the card. Either 0, 1, 2, or 3. It is set to -1 by default.
    bool flipped = false; // Turns true if the card has been flipped by the player.
    int flipping = 0;     // Turns to 1 if the card is currently in the process of flipping, turns to 2 if the card has flipped but isn't back to its normal scale.

    int front_id;   // The selected card front color.
    int back_id;    // The selected card back color.
    int pattern_id; // The selected card back pattern.

    sf::RenderTexture rendertexture;  // The card's texture.
    sf::Sprite sprite;                // The card's sprite.

    // The bools in this array trurn true if the player used the memo to mark the card as the corresponding number. If marked[0] is true, the card has been marked as a 0 by the player.
    bool marked[4] = {
        false,
        false,
        false,
        false
    };
    // The bools in this array are set to true if the texture has been applied the corresponding mark number. If current_texture[0] is true, the card's texture has been updated to show a yellow 0 on it.
    bool current_texture[4] = {
        false,
        false,
        false,
        false
    };


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

    // Reloads the card's textures.
    void reloadTextures(int front, int back, int pattern);
};



class Indicator
{
public:
    int index = -1;    // This is the index of the row/column of the indicator.

    int card_sum = -1; // The sum of the values of the cards that are on the row/column of this indicator.
    int zero_num = -1; // The number of zeros that are on the row/column of the indicator.

    sf::Texture texture; // The indicator's texture.
    sf::Sprite sprite;   // The indicator's sprite.

    sf::Texture num_texture;             // The texture for each number on the indicator.
    sf::Sprite num_sprite;               // The sprite for each number on the indicator.
    sf::RenderTexture num_rendertexture; // The rendertexture for each number on the indicator.
    int num_pos[3][2] = {                // The position of position of each number on the indicator.
        {11, 2}, 
        {19, 2}, 
        {19, 15}
    };


    // Gives the indicator a texture and sprite.
    Indicator();

    // Sets the row/colmns of the indicator.
    void set_index(int new_index);

    // Renders the indicator's sprite onto the game window.
    void render(sf::RenderWindow& window);

    // Resets the indicator.
    void reset(int sum_of_cards, int num_of_zeros);
};



class GameBoard
{
public:
    Card board[5][5];          // This is the game board that holds all the flippable cards.
    Indicator indicator[2][5]; // This is the array that holds all the indicators.
    int size = 5;              // This is the width and height of the game board. Meaning it holds 5 rows of 5 cards.

    bool is_game_over = false; // This turns true if the player flipped a 0.

    sf::Texture between_cards_texture; // Texture for the lines in between the cards.
    sf::Sprite  between_cards_sprite;  // Sprite  for the lines in between the cards.


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

    // Reloads the card textures with the new cosmetic ids.
    void reloadCardTextures(int front, int back, int pattern);
};



class Shop
{
public:
    sf::Texture pannel_texture; // Texture used for the shop.
    sf::Sprite  pannel_sprite;  // Sprite  used for the shop.

    int selected_items[3] = { 0, 0, 0 }; // The index of each of the selected items.

    sf::Texture highlight_t;    // The texture of the highlight around the selected item.
    sf::Sprite  highlight_s[3]; // The sprite  of the highlight around the selected item.

    sf::Texture items_t[3][5]; // The textures of all of the shop items.
    sf::Sprite  items_s[3][5]; // The sprites  of all of the shop items.


    // Constructor that initializes the shop and its items.
    Shop();

    // Checks if the player changed his cosmetics. Returns true if he did.
    bool update(sf::RenderWindow& window);

    // Renders the shop and its items.
    void render(sf::RenderWindow& window);
};



class Ui
{
public:
    sf::RenderWindow window; // Game window onto which all game objects are rendered.
    Billy billy;             // Billy the pumpkin.
    Shop shop;               // The game's shop.
    GameBoard game_board;    // Game board that holds all the flippable cards.
    sf::Clock global_clock;  // Initialize the global internal clock of the game.

    int lv          = 1; // This value determines the difficulty of the game board.
    int total_score = 0; // Total score of the user, to which the score of each round is added.
    int game_score  = 1; // Player's score in the current game, which gets multiplied by the value of every card flipped.

    // The keys the player can use to mark cards.
    sf::Keyboard::Key keybindings[4] = {
        sf::Keyboard::Numpad0,
        sf::Keyboard::Numpad1,
        sf::Keyboard::Numpad2,
        sf::Keyboard::Numpad3,
    };

    int last_flip_time  = -100; // This is the last time a card was flipped.
    int last_mark_time  = -100; // This is the last time the player marked a card with the memo.
    int game_end_time   = -100; // This is the time when the user flips a 0.
    int last_reset_time = -100; // This is the last time when the game was reset.

    sf::Font    font;             // The font used for all text in the ui.
    sf::Text    title_text[2];    // Shows the game's name.
    sf::Text    total_score_text; // Shows the player's total score.
    sf::Text    game_score_text;  // Shows the player's score in the current game.
    sf::Text    game_lv_text;     // Shows the level of the player's current game.

    sf::Texture cursor_texture; // Texture used for the mouse cursor.
    sf::Sprite  cursor_sprite;  // Sprite  used for the mouse cursor.

    sf::Texture highlight_texture;   // Texture for the card hover highlight.
    sf::Sprite  highlight_sprite;    // Sprite  for the card hover highlight.
    int         highlight_alpha = 0; // Set to 0 if the mouse is !on a card. Increases to 255 when it is.

    sf::Texture scoreboard_texture; // Texture used for the scoreboard.
    sf::Sprite  scoreboard_sprite;  // Sprite  used for the scoreboard.


    // Constructor that initializes the game window.
    Ui();

    // Processes user inputs and calls the corresponding functions.
    void input_logic();

    // Applies the game rules every frame.
    void game_logic();

    // Renders all the game objects onto the game window.
    void render();
};