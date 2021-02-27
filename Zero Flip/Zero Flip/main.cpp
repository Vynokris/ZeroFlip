// Press alt+F7, go to Debugging Settings and set Environment to "PATH=%PATH%;$(ProjectDir)\Resources\dll"
// To use windows.h, press alt+F7, go to "C/C++", then "Language" and set "Disable language extensions" to "No"


/*
TODO:
- Make it so the game becomes increasingly more difficult with the increasing level. (Make it so that 1s are less and less common with the rising levels).
*/


#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <iostream>
#include <ctime>


#include "Resources/Classes/classes.h"



int main()
{
	// Initialize the random number seed to get different rolls everytime the game starts.
	srand(int(time(0)));

	// Initialize the variable that holds the last time the game loop was executed.
	int last_loop_time = -5;

	// Initialize the ui object.
	Ui ui;

	ui.game_board.reset(1);

	/*
	// Marks all the cards' values onto their backs.
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			ui.game_board.board[i][j].mark_as(ui.game_board.board[i][j].val);
		}
	}*/


	while (true)
	{
		// Stop the execution if the window is closed.
		if (not ui.window.isOpen()) {
			return 0;
		}


		// Game loop.
		if (ui.global_clock.getElapsedTime().asMilliseconds() - last_loop_time >= 5) { // This makes the minimal frame duration 5 milliseconds, meaning the game is capped at 200 fps.
			last_loop_time = ui.global_clock.getElapsedTime().asMilliseconds();

			ui.input_logic();

			ui.game_logic();

			ui.render();
		}
	}

	return 0;
}