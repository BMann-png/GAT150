#pragma once
#include "Engine.h"
#include <iostream>
#include <SDL.h>
#include <SDL_Image.h>
#include <cassert>

class Game
{
public:
	void Initialize();
	void Shutdown();
	
	void Update();
	void Draw();

	bool IsQuit() { return quit; }

private:
	

public:
	std::unique_ptr<pbls::Engine> engine;
	std::unique_ptr<pbls::Scene> scene;

private:
	bool quit{ false };
	
};