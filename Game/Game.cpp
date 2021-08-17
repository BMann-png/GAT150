#include "Game.h"
#include "Actors/Player.h"
#include "Actors/Enemy.h"

void Game::Initialize()
{
	//create engine
	engine = std::make_unique<pbls::Engine>();
	engine->Startup();
	engine->Get<pbls::Renderer>()->Create("GAT150", 800, 600);

	//create scene
	scene = std::make_unique<pbls::Scene>();
	scene->engine = engine.get();

	pbls::SeedRandom(static_cast<unsigned int>(time(nullptr)));
	pbls::SetFilePath("../Resources");

	//int size = 24;
	//std::shared_ptr<pbls::Font> font = engine->Get<pbls::ResourceSystem>()->Get<pbls::Font>("Fonts/Axel Gilby Comic Sans.ttf", &size);
	//std::shared_ptr<pbls::Texture> textTexture = std::make_shared<pbls::Texture>(engine->Get<pbls::Renderer>());
	//textTexture->Create(font->CreateSurface("hello world", pbls::Color::orange));
	//engine->Get<pbls::ResourceSystem>()->Add("textTexture", textTexture);

	
	engine->Get<pbls::AudioSystem>()->AddAudio("explosion", "Audio/Explosion.wav");
	engine->Get<pbls::AudioSystem>()->AddAudio("windowsxp", "Audio/windowsxp/Windows XP Startup.wav");
	engine->Get<pbls::AudioSystem>()->AddAudio("FrenchyJazz", "Audio/windowsxp/Banana Man.mp3");
	musicChannel = engine->Get<pbls::AudioSystem>()->PlayAudio("frenchyjazz", 1, 1, true);


	std::shared_ptr<pbls::Texture> texture = engine->Get<pbls::ResourceSystem>()->Get<pbls::Texture>("sf2.png", engine->Get<pbls::Renderer>());
	engine->Get<pbls::ResourceSystem>()->Add("explosion1", engine->Get<pbls::ResourceSystem>()->Get<pbls::Texture>("particle01.png", engine->Get<pbls::Renderer>()));
	engine->Get<pbls::ResourceSystem>()->Add("explosion2", engine->Get<pbls::ResourceSystem>()->Get<pbls::Texture>("particle02.png", engine->Get<pbls::Renderer>()));

	//for (size_t i = 0; i < 10; i++)
	//{

	//	pbls::Transform transform{ {pbls::RandomRange(0, 800), pbls::RandomRange(0, 600)}, pbls::RandomRange(0, 360), 1.0f };
	//	std::unique_ptr<pbls::Actor> actor = std::make_unique<pbls::Actor>(transform, texture);
	//	scene->AddActor(std::move(actor));

	//}

	// game
	engine->Get<pbls::AudioSystem>()->AddAudio("playerFire", "playerFire.wav");
	engine->Get<pbls::AudioSystem>()->AddAudio("enemyFire", "enemyFire.wav");
	engine->Get<pbls::AudioSystem>()->AddAudio("backgroundMusic", "GameLoop.mid");
	
	engine->Get<pbls::EventSystem>()->Subscribe("AddPoints", std::bind(&Game::OnAddPoints, this, std::placeholders::_1));
	engine->Get<pbls::EventSystem>()->Subscribe("PlayerHit", std::bind(&Game::OnPlayerHit, this, std::placeholders::_1));

}

void Game::Shutdown()
{
	scene->RemoveAllActors();
	engine->Shutdown();
}

void Game::Update()
{
	engine->Update();

	float dt = engine->time.deltaTime;

	stateTimer += dt;

	switch (state)
	{
	case Game::eState::Title:
		UpdateTitle(dt);
		break;
	case Game::eState::StartGame:
		score = 0;
		lives = 3;

		state = eState::StartLevel;
		break;
	case Game::eState::StartLevel:
		UpdateLevelStart(dt);
		break;
	case Game::eState::Game:
		GameLoop();

		if (lives <= 0)
		{
			state = eState::GameOver;
		}
		break;
	case Game::eState::GameOver:

		scene->RemoveAllActors();
		
		if (engine->Get<pbls::InputSystem>()->GetKeyState(SDL_SCANCODE_SPACE) == pbls::InputSystem::eKeyState::Pressed)
		{
			state = eState::Title;
		}

		break;
	default:
		break;
	}

	// update

	quit = (engine->Get<pbls::InputSystem>()->GetKeyState(SDL_SCANCODE_ESCAPE) == pbls::InputSystem::eKeyState::Pressed);

	//if (engine->Get<pbls::InputSystem>()->GetButtonState((int)pbls::InputSystem::eMouseButton::Left) == pbls::InputSystem::eKeyState::Pressed)
	//{
	//	pbls::Vector2 position = engine->Get<pbls::InputSystem>()->GetMousePosition();
	//	std::cout << position.x << " " << position.y << std::endl;
	//	//Create particles
	//	engine->Get<pbls::ParticleSystem>()->Create(position, 5, 3, engine->Get<pbls::ResourceSystem>()->Get<pbls::Texture>("explosion1", engine->Get<pbls::Renderer>()) , 100);
	//	engine->Get<pbls::ParticleSystem>()->Create(position, 4, 3, engine->Get<pbls::ResourceSystem>()->Get<pbls::Texture>("explosion2", engine->Get<pbls::Renderer>()) , 100);

	//	musicChannel.SetPitch(musicChannel.GetPitch() + 0.1f);
	//}
	scene->Update(dt);
}

void Game::Draw()
{
	engine->Get<pbls::Renderer>()->BeginFrame();


	int size = 24;
	std::shared_ptr<pbls::Font> font = engine->Get<pbls::ResourceSystem>()->Get<pbls::Font>("Fonts/Axel Gilby Comic Sans.ttf", &size);

	pbls::Transform t1;
	pbls::Transform t2;
	pbls::Transform t3;
	pbls::Transform t4;
	pbls::Transform t5;

	std::shared_ptr<pbls::Texture> scoreTexture = std::make_shared<pbls::Texture>(engine->Get<pbls::Renderer>());
	scoreTexture->Create(font->CreateSurface(std::to_string(score), pbls::Color::green));
	engine->Get<pbls::ResourceSystem>()->Add("score", scoreTexture);

	std::shared_ptr<pbls::Texture> livesTexture = std::make_shared<pbls::Texture>(engine->Get<pbls::Renderer>());
	livesTexture->Create(font->CreateSurface(std::to_string(lives), pbls::Color::blue));
	engine->Get<pbls::ResourceSystem>()->Add("lives", livesTexture);

	std::shared_ptr<pbls::Texture> bombsTexture = std::make_shared<pbls::Texture>(engine->Get<pbls::Renderer>());
	bombsTexture->Create(font->CreateSurface(std::to_string(bombs), pbls::Color::red));
	engine->Get<pbls::ResourceSystem>()->Add("bombs", bombsTexture);

	std::shared_ptr<pbls::Texture> roundTexture = std::make_shared<pbls::Texture>(engine->Get<pbls::Renderer>());
	roundTexture->Create(font->CreateSurface(std::to_string(round), pbls::Color::white));
	engine->Get<pbls::ResourceSystem>()->Add("round", roundTexture);

	std::shared_ptr<pbls::Texture> menuTexture = std::make_shared<pbls::Texture>(engine->Get<pbls::Renderer>());
	menuTexture->Create(font->CreateSurface("Press Space to Start", pbls::Color::white));
	engine->Get<pbls::ResourceSystem>()->Add("menu", menuTexture);

	std::shared_ptr<pbls::Texture> gameOverTexture = std::make_shared<pbls::Texture>(engine->Get<pbls::Renderer>());
	gameOverTexture->Create(font->CreateSurface("Press Space to Go to Main Menu", pbls::Color::white));
	engine->Get<pbls::ResourceSystem>()->Add("gameOver", gameOverTexture);
	switch (state)
	{
	case Game::eState::Title:
		
		t1.position = { 400, 300 };
		engine->Get<pbls::Renderer>()->Draw(engine->Get<pbls::ResourceSystem>()->Get<pbls::Texture>("menu", engine->Get<pbls::Renderer>()), t1);

		break;
	case Game::eState::StartGame:
		break;
	case Game::eState::StartLevel:
		break;
	case Game::eState::Game:
		
		t5.position = { 50, 30 };
		engine->Get<pbls::Renderer>()->Draw(engine->Get<pbls::ResourceSystem>()->Get<pbls::Texture>("round", engine->Get<pbls::Renderer>()), t5);

		
		t2.position = { 50, 60 };
		engine->Get<pbls::Renderer>()->Draw(engine->Get<pbls::ResourceSystem>()->Get<pbls::Texture>("score", engine->Get<pbls::Renderer>()), t2);

		
		t3.position = { 750, 30 };
		engine->Get<pbls::Renderer>()->Draw(engine->Get<pbls::ResourceSystem>()->Get<pbls::Texture>("lives", engine->Get<pbls::Renderer>()), t3);

		
		t4.position = { 750, 60 };
		engine->Get<pbls::Renderer>()->Draw(engine->Get<pbls::ResourceSystem>()->Get<pbls::Texture>("bombs", engine->Get<pbls::Renderer>()), t4);

		break;
	case Game::eState::GameOver:
		
		t1.position = { 400, 300 };
		engine->Get<pbls::Renderer>()->Draw(engine->Get<pbls::ResourceSystem>()->Get<pbls::Texture>("gameOver", engine->Get<pbls::Renderer>()), t1);


		break;
	default:
		break;

		
	}

	engine->Draw(engine->Get<pbls::Renderer>());
	scene->Draw(engine->Get<pbls::Renderer>());

	//pbls::Transform t;
	//t.position = { 30, 30 };
	//engine->Get<pbls::Renderer>()->Draw(engine->Get<pbls::ResourceSystem>()->Get<pbls::Texture>("textTexture", engine->Get<pbls::Renderer>()), t);

	engine->Get<pbls::Renderer>()->EndFrame();

}

void Game::OnAddPoints(const pbls::Event& event)
{
	score += std::get<int>(event.data);
}
void Game::OnPlayerHit(const pbls::Event& event)
{
	lives--;
	std::cout << std::get<std::string>(event.data) << std::endl;
}

void Game::GameLoop()
{
	if (scene->GetActors<Enemy>().size() == 0)
	{
		int numberOfEnemies = pbls::RandomRangeInt(1, 10);
		for (size_t i = 0; i < numberOfEnemies; i++)
		{
			scene->AddActor(std::make_unique<Enemy>(pbls::Transform{ { pbls::RandomRange(0.0f, 800.0f), pbls::RandomRange(10.0f, 20.0f) }, pbls::RandomRange(0.0f, pbls::TwoPi), 0.5f }, engine->Get<pbls::ResourceSystem>()->Get<pbls::Texture>("Textures/ship.png", engine->Get<pbls::Renderer>()), 200));
		}
		round++;
	}
	bombs = scene->GetActor<Player>()->getBombs();
}

void Game::UpdateTitle(float dt)
{
	if (engine->Get<pbls::InputSystem>()->GetKeyState(SDL_SCANCODE_SPACE) == pbls::InputSystem::eKeyState::Pressed)
	{
		state = eState::StartGame;
	}
}

void Game::UpdateLevelStart(float dt)
{
	{
		scene->AddActor(std::make_unique<Player>(pbls::Transform{ { 400, 300 }, 0, 0.75f }, engine->Get<pbls::ResourceSystem>()->Get<pbls::Texture>( "Textures/ship.png", engine->Get<pbls::Renderer>()), 500));
		for (size_t i = 0; i < 3; i++)
		{
			scene->AddActor(std::make_unique <Enemy>(pbls::Transform{ { pbls::RandomRange(0.0f, 800.0f), pbls::RandomRange(0.0f, 600.0f) }, pbls::RandomRange(0.0f, pbls::TwoPi), 0.5f }, engine->Get<pbls::ResourceSystem>()->Get<pbls::Texture>("Textures/ship.png", engine->Get<pbls::Renderer>()), 200));
		}

		state = eState::Game;
	}
}
