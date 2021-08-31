#include "Game.h"
#include <fstream>

int main(int, char**)

{
	Game game;
	game.Initialize();

	bool quit = false;
	SDL_Event event;
	//float quitTime = engine.time.time + 3.0f;

	while (!quit && !game.IsQuit())
	{
		SDL_PollEvent(&event);
		switch(event.type)
		{
			case SDL_QUIT:
				quit = true;
				break;
		}

		game.Update();
		game.Draw();
	}


	SDL_Quit();

	return 0; 

}