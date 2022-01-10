// Press alt+F7, go to Debugging Settings and set Environment to "PATH=%PATH%;$(ProjectDir)\Resources\dll"
// To use windows.h, press alt+F7, go to "C/C++", then "Language" and set "Disable language extensions" to "No"


/*
TODO:
- Create and animate Billy the Pumpkin.
- Tweak the level generation to balance it a bit more.
*/


#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <iostream>
#include <ctime>
#include <fstream>

#include "classes.hpp"



int main()
{
    // Initialize the random number seed to get different rolls everytime the game starts.
    srand(int(time(0)));

    // Initialize the variable that holds the last time the game loop was executed.
    int last_loop_time = -5;

    // Initialize the ui object.
    Ui ui;

    while (true)
    {
        // Stop the execution if the window is closed.
        if (!ui.window.isOpen() || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) 
        {
            // Save the player's total score.
            std::ofstream out("Resources/PlayerData/coins.txt");
            out << ui.total_score;
            out.close();
            break;
        }

        // Game loop.
        if (ui.window.hasFocus() &&
            ui.global_clock.getElapsedTime().asMilliseconds() - last_loop_time >= 5) // This makes the minimal frame duration 5 milliseconds, meaning the game is capped at 200 fps.
        { 
            last_loop_time = ui.global_clock.getElapsedTime().asMilliseconds();

            ui.input_logic();

            ui.game_logic();

            ui.render();
        }
    }

    return 0;
}