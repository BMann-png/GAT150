#include "Game.h"
#include "GameComponent/PlayerComponent.h"
#include "GameComponent/EnemyComponent.h"
#include "GameComponent/PickupComponent.h"
#include "GameComponent/ProjectileComponent.h"

void Game::Initialize()
{
	//create engine
	engine = std::make_unique<pbls::Engine>();
	engine->Startup();
	engine->Get<pbls::Renderer>()->Create("GAT150", 800, 600);
	
	//register components
	REGISTER_CLASS(PlayerComponent);
	REGISTER_CLASS(EnemyComponent);
	REGISTER_CLASS(PickupComponent);
	REGISTER_CLASS(ProjectileComponent);


	//create scene
	scene = std::make_unique<pbls::Scene>();
	scene->engine = engine.get();

	pbls::SeedRandom(static_cast<unsigned int>(time(nullptr)));
	pbls::SetFilePath("../Resources");

	engine->Get<pbls::EventSystem>()->Subscribe("onAddScore", std::bind(&Game::OnAddScore, this, std::placeholders::_1));

}

void Game::Shutdown()
{
	scene->RemoveAllActors();
	engine->Shutdown();
}

void Game::Update()
{
	engine->Update();

	quit = (engine->Get<pbls::InputSystem>()->GetKeyState(SDL_SCANCODE_ESCAPE) == pbls::InputSystem::eKeyState::Pressed);

	switch (state)
	{
	case Game::eState::Reset:
		Reset();
		break;
	case Game::eState::Title:
		Title();
		break;
	case Game::eState::StartGame:
		StartGame();
		break;
	case Game::eState::StartLevel:
		StartLevel();
		break;
	case Game::eState::Level:
		Level();
		break;
	case Game::eState::PlayerDead:
		PlayerDead();
		break;
	case Game::eState::GameOver:
		GameOver();
		break;
	default:
		break;
	}

	//update score
	auto scoreActor = scene->FindActor("Score");
	if(scoreActor) scoreActor->GetComponent<pbls::TextComponent>()->SetText("Score: " + std::to_string(score));

	scene->Update(engine->time.deltaTime);
}

void Game::Draw()
{
	engine->Get<pbls::Renderer>()->BeginFrame();

	engine->Draw(engine->Get<pbls::Renderer>());
	scene->Draw(engine->Get<pbls::Renderer>());

	engine->Get<pbls::Renderer>()->EndFrame();

}

void Game::Reset()
{
	scene->RemoveAllActors();

	rapidjson::Document document;
	bool success = pbls::json::Load("title.txt", document);
	assert(success);

	scene->Read(document);

	state = eState::Title;
}

void Game::Title()
{
	if (engine->Get<pbls::InputSystem>()->GetKeyState(SDL_SCANCODE_SPACE) == pbls::InputSystem::eKeyState::Pressed)
	{
		auto title = scene->FindActor("Title");
		assert(title);
		title->Active = false;
		state = eState::StartGame;
	}
}

void Game::StartGame()
{
	score = 0;

	rapidjson::Document document;
	bool success = pbls::json::Load("scene.txt", document);
	assert(success);

	scene->Read(document);

	pbls::Tilemap tilemap;
	tilemap.scene = scene.get();
	success = pbls::json::Load("map.txt", document);
	assert(success);

	tilemap.Read(document);
	tilemap.Create();

	state = eState::StartLevel;
	stateTimer = 0;
}

void Game::StartLevel()
{
	stateTimer += engine->time.deltaTime;

	if (stateTimer >= 1)
	{
		auto player = pbls::ObjectFactory::Instance().Create<pbls::Actor>("Player");
		player->transform.position = { 400, 300 };
		scene->AddActor(std::move(player));
		state = eState::Level;
	}
}

void Game::Level()
{
	spawnTimer -= engine->time.deltaTime;
	if (spawnTimer <= 0)
	{
		spawnTimer = pbls::RandomRange(0.5f, 2.0f);

		if (pbls::RandomRangeInt(0, 1) == 1)
		{
			auto coin = pbls::ObjectFactory::Instance().Create<pbls::Actor>("Coin");
			coin->transform.position = pbls::Vector2{ pbls::RandomRange(50, 750), pbls::RandomRange(100, 400) };
			scene->AddActor(std::move(coin));
		}
		else
		{
			auto enemy = pbls::ObjectFactory::Instance().Create<pbls::Actor>("Enemy");
			enemy->transform.position = pbls::Vector2{ pbls::RandomRange(50, 750), pbls::RandomRange(100, 400) };
			scene->AddActor(std::move(enemy));
		}
	}
	if (scene->FindActor("Player") == nullptr) state = eState::PlayerDead;
}

void Game::PlayerDead()
{
	auto title = scene->FindActor("Title");
	assert(title);
	title->Active = true;
	title->GetComponent<pbls::TextComponent>()->SetText("Game over, press space to go back to main menu");
	state = eState::GameOver;
}

void Game::GameOver()
{
	if (engine->Get<pbls::InputSystem>()->GetKeyState(SDL_SCANCODE_SPACE) == pbls::InputSystem::eKeyState::Pressed)
	{
		state = eState::Reset;
	}
}

void Game::OnAddScore(const pbls::Event& event)
{
	score += std::get<int>(event.data);
}
