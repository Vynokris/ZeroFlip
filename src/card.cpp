#include "classes.hpp"



// Gives the card a texture and a sprite.
Card::Card() 
{
    // texture.loadFromFile("Resources/Art/Card/Back/back" + std::to_string(back_id) + ".png");

    // Create the card's render texture.
    rendertexture.create(28, 28);
    rendertextureDraw(rendertexture, "Resources/Art/Card/Back/back" + std::to_string(back_id) + ".png", 0, 0);

    // Create the card's sprite.
    sprite.setTexture(rendertexture.getTexture());
    sprite.setScale(SCALE, SCALE);
}



// Flips the card over.
void Card::flip(int delay)
{
    if (delay > 0) {
        flipDelay = delay;
        flipState = FlipStates::PreFlip;
    }
    else {
        flipState = FlipStates::FlipStart;
    }
}



// Marks the card as either a 0, a 1, a 2 or a 3.
void Card::mark_as(int num)
{
    if (!flipped) {
        // Invert the mark on the card for the given number.
        marked[num] = !marked[num];

        // Choose the right texture texture.
        std::string texturePath;
        if (marked[num]) 
            texturePath = "Resources/Art/Card/Markings/marked" + std::to_string(num) + ".png";
        else 
            texturePath = "Resources/Art/Card/Back/unmark" + std::to_string(back_id) + ".png";
        
        // Paste it onto the card rendertexture in the right place.
        switch (num)
        {
        case 0:
            rendertextureDraw(rendertexture, texturePath, 4, 4);
            break;
        case 1:
            rendertextureDraw(rendertexture, texturePath, 20, 4);
            break;
        case 2:
            rendertextureDraw(rendertexture, texturePath, 4, 19);
            break;
        case 3:
            rendertextureDraw(rendertexture, texturePath, 20, 19);
            break;
        default:;
        }
        sprite.setTexture(rendertexture.getTexture());
    }
}



// Renders the card's sprite onto the game window.
void Card::render(sf::RenderWindow& window)
{
    // Decrease the flip delay.
    if (flipDelay != -1) {
        flipDelay--;
    }
    if (flipDelay == 0) {
        flipDelay = -1;
        flipState = FlipStates::FlipStart;
    }

    // Animate the card flipping.
    if (flipState != FlipStates::Normal) 
    {
        // When the card hasn't flipped but is in the process of doing so.
        if (sprite.getScale().x > 0 && flipState == FlipStates::FlipStart) 
        {
            sprite.setScale(sf::Vector2f(float(sprite.getScale().x - 0.2), SCALE));
            sprite.setPosition(sf::Vector2f(float(sprite.getPosition().x + 0.665 * SCALE), sprite.getPosition().y));
        }
        // When the card flips.
        else if (sprite.getScale().x <= 0) 
        {
            flipState = FlipStates::Flipping;
            flipped = !flipped;
            sprite.setScale(sf::Vector2f(float(sprite.getScale().x + 0.2), SCALE));
            sprite.setPosition(sf::Vector2f(float(sprite.getPosition().x - 0.665 * SCALE), sprite.getPosition().y));

            // Change the card texture.
            if (flipped) 
            {
                rendertextureDraw(rendertexture, "Resources/Art/Card/Front/front" + std::to_string(front_id) + ".png", 0, 0);
                rendertextureDraw(rendertexture, "Resources/Art/Card/Front/num"   + std::to_string(val)      + ".png", 10, 9);
            }
            else 
            {
                rendertextureDraw(rendertexture, "Resources/Art/Card/Back/back"       + std::to_string(back_id)    + ".png", 0, 0);
                rendertextureDraw(rendertexture, "Resources/Art/Card/Pattern/pattern" + std::to_string(pattern_id) + ".png", 6, 6);
            }
            sprite.setTexture(rendertexture.getTexture());
        }
        // When the card starts flipping the other way.
        else if (flipState == FlipStates::Flipping)
        {
            flipState = FlipStates::FlipEnd;
        }
        // When the card has finished flipping.
        else if (sprite.getScale().x >= SCALE && flipState == FlipStates::FlipEnd) 
        {
            flipState = FlipStates::Normal;
        }
        // When the card hasn't finished flipping yet but is in the process of doing so.
        else if (sprite.getScale().x > 0 && flipState == FlipStates::FlipEnd) 
        {
            sprite.setScale(sf::Vector2f(float(sprite.getScale().x + 0.2), SCALE));
            sprite.setPosition(sf::Vector2f(float(sprite.getPosition().x - 0.665 * SCALE), sprite.getPosition().y));
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

    /*
    if (flipped) {
        flip();
    }
    else {
        rendertextureDraw(rendertexture, "Resources/Art/Card/Back/back" + std::to_string(back_id) + ".png", 0, 0);
        rendertextureDraw(rendertexture, "Resources/Art/Card/Pattern/pattern" + std::to_string(pattern_id) + ".png", 6, 6);
        sprite.setTexture(rendertexture.getTexture());
    }
    */
    
}



// Reloads the card's textures.
void Card::reloadTextures(int front, int back, int pattern)
{
    front_id   = front;
    back_id    = back;
    pattern_id = pattern;

    // Change the card texture.
    if (flipped) 
    {
        rendertextureDraw(rendertexture, "Resources/Art/Card/Front/front" + std::to_string(front_id) + ".png", 0, 0);
        rendertextureDraw(rendertexture, "Resources/Art/Card/Front/num"   + std::to_string(val)      + ".png", 10, 9);
    }
    else 
    {
        rendertextureDraw(rendertexture, "Resources/Art/Card/Back/back" + std::to_string(back_id) + ".png", 0, 0);
        rendertextureDraw(rendertexture, "Resources/Art/Card/Pattern/pattern" + std::to_string(pattern_id) + ".png", 6, 6);
    }
    sprite.setTexture(rendertexture.getTexture());

    // Add the markings back on the card.
    sf::Vector2i markingPos[4] = { { 4, 4 }, { 20, 4 }, { 4, 19 }, { 20, 19 } };
    for (int i = 0; i < 4; i++)
    {
        if (marked[i])
            rendertextureDraw(rendertexture, "Resources/Art/Card/Markings/marked" + std::to_string(i) + ".png", markingPos[i].x, markingPos[i].y);
    }
}