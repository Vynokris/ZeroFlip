#include "classes.hpp"

Billy::Billy()
{
    // Create billy's rendertexture.
    rendertexture.create(16, 16);

    // Link billy's sprite to his texture.
    sprite.setTexture(rendertexture.getTexture());
    sprite.setScale(SCALE, SCALE);
    
    reloadTexture();
}

void Billy::addEvent(GameEvent eventType, std::vector<int> data)
{

}

void Billy::reloadTexture()
{
    rendertexture.clear(sf::Color(0, 0, 0, 0));
    rendertextureDraw(rendertexture, "Resources/Art/Billy/pumpkin" + std::to_string(current_texture) + ".png", 0, 0);
    // TODO: draw billy's face too.
    rendertexture.display();
}

void Billy::update()
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
}

void Billy::render(sf::RenderWindow& window)
{
    // Draw billy's sprite.
    window.draw(sprite);
}