// Press alt+F7, go to Debugging Settings and set Environment to "PATH=%PATH%;$(ProjectDir)\Resources\dll"
// To use windows.h, press alt+F7, go to "C/C++", then "Language" and set "Disable language extensions" to "No"


/*
TODO:
- Fix the game end glitch (coins don't get updated and level doesn't increase)
- Fix the first board reload glitch (it keeps the same values as before)
- Create and animate Billy the Pumpkin.
- Tweak the level generation to balance it a bit more.
*/

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

        // Game loop (capped at 75 fps).
        if (ui.window.hasFocus() && ui.global_clock.getElapsedTime().asSeconds() - last_loop_time > DELTA_TIME)
        { 
            last_loop_time = ui.global_clock.getElapsedTime().asSeconds();

            ui.input_logic();

            ui.game_logic();

            ui.render();
        }
    }

    return 0;
}