#include "classes.hpp"

Billy::Billy()
{
    // Create billy's rendertexture.
    rendertexture.create(16, 16);

    // Link billy's sprite to his texture.
    sprite.setTexture(rendertexture.getTexture());
    sprite.setScale(SCALE * 1.5, SCALE * 1.5);

    // Link billy's text texture to his text sprite.
    textTexture.loadFromFile("Resources/Art/Billy/Text/text0.png");
    textSprite.setTexture(textTexture);
    textSprite.setScale(SCALE * 1.5, SCALE * 1.5);
    
    reloadTexture();
}

void Billy::addEvent(GameEvents eventType, std::vector<int> data)
{
    gameEvents.insert(gameEvents.begin(), {eventType, data});
    if (gameEvents.size() > 10)
        gameEvents.pop_back();
}

void Billy::reloadTexture()
{
    // Reload billy's texture.
    rendertexture.clear(sf::Color(0, 0, 0, 0));
    rendertextureDraw(rendertexture, "Resources/Art/Billy/pumpkin" + std::to_string(current_texture) + ".png", 0, 0);
    rendertextureDraw(rendertexture, "Resources/Art/Billy/Face/face" + std::to_string((int)emotion) + ".png", 0, current_texture);
    rendertexture.display();

    // Reload billy's text.
    if (current_text >= 0)
    {
        textTexture.loadFromFile("Resources/Art/Billy/Text/text" + std::to_string(current_text) + ".png");
        textSprite.setPosition(textSprite.getPosition().x, sprite.getPosition().y - textSprite.getGlobalBounds().height + (current_texture + 2) * SCALE * 1.5);
    }
}

void Billy::update(sf::RenderWindow& window)
{
    static int counter = -1;
    counter++;

    // Update billy's texture based on the frame counter.
    if (counter > 60)
    {
        counter = -1;
        current_texture = (current_texture + 1) % 2;
        reloadTexture();
    }

    // Reset billy's emotion after the cooldown ends.
    if (emotion_cooldown)
        emotion_cooldown--;
    else if (emotion != BillyEmotions::NORMAL)
    {
        emotion = BillyEmotions::NORMAL;    
        current_text = -1;
    }

    // Check if the player clicked billy.
    sf::Vector2 mouse_pos = sf::Mouse::getPosition(window);
    sf::Vector2 mouse_world_pos = window.mapPixelToCoords(mouse_pos);
    if (sprite.getGlobalBounds().contains(mouse_world_pos))
    {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            addEvent(GameEvents::CLICKED_BILLY, std::vector<int>{0});
            emotion_cooldown = 180;
        }
    }
    
    // Check the latest events to update billy's emotions.
    if (gameEvents.size() > 0)
    {
        // Squish billy if the player clicked him.
        if (gameEvents[0].first == GameEvents::CLICKED_BILLY && gameEvents[0].second[0] == 0)
        {
            addEvent(GameEvents::CLICKED_BILLY, std::vector<int>{1});
            emotion = BillyEmotions::SMALL_FACE;
            current_texture = 2;
            counter = -1;
            reloadTexture();
        }

        // ---- End of game ---- //
        if (gameEvents[0].first == GameEvents::END_GAME)
        {
            // Make billy happy if the player won.
            if (gameEvents[0].second[0] == 1)
            {
                emotion = BillyEmotions::HAPPY;
                current_text = 1;
            }
            // Make billy sad if the player lost.
            else
            {
                emotion = BillyEmotions::CRYING;
                current_text = 0;
            }
            reloadTexture();
        }

        // ---- Start of game ---- //
        if (gameEvents[0].first == GameEvents::START_GAME)
        {
            emotion = BillyEmotions::NORMAL;
            reloadTexture();
        }
    }
}

void Billy::render(sf::RenderWindow& window)
{
    // Draw billy's sprite.
    window.draw(sprite);

    // Draw billy's text.
    if (current_text >= 0)
        window.draw(textSprite);
}