#pragma once
#include "Engine.h"
#include <iostream>
#include <SDL.h>
#include <SDL_Image.h>
#include <cassert>

class Game
{
public:
	enum class eState
	{
		Title,
		StartGame,
		StartLevel,
		Game,
		GameOver,
	};
public:
	void Initialize();
	void Shutdown();
	
	void Update();
	void Draw();

	bool IsQuit() { return quit; }

private:
	void UpdateTitle(float dt);
	void UpdateLevelStart(float dt);
	void OnAddPoints(const pbls::Event& event);
	void OnPlayerHit(const pbls::Event& event);
	void GameLoop();

public:
	std::unique_ptr<pbls::Engine> engine;
	std::unique_ptr<pbls::Scene> scene;

private:
	bool quit{ false };
	eState state = eState::Title;
	float stateTimer{ 0.0f };

	size_t score{ 0 };
	size_t lives{ 0 };
	size_t bombs{ 0 };
	size_t round{ 0 };

	pbls::AudioChannel musicChannel;

	std::string roundDisplay{};
	std::string highscore{};
};