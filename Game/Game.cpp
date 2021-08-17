#include "Game.h"

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

	//actors
	std::unique_ptr<pbls::Actor> actor = std::make_unique<pbls::Actor>(pbls::Transform{ {400, 300} });

	{
		std::unique_ptr<pbls::SpriteComponent> component = std::make_unique<pbls::SpriteComponent>();
		component->texture = engine->Get<pbls::ResourceSystem>()->Get<pbls::Texture>("sf2.png", engine->Get<pbls::Renderer>());
		actor->AddComponent(std::move(component));
	}
	{
		std::unique_ptr<pbls::PhysicsComponent> component = std::make_unique<pbls::PhysicsComponent>();
		component->ApplyForce(pbls::Vector2::right * 200);
		actor->AddComponent(std::move(component));
	}

	scene->AddActor(std::move(actor));

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

	scene->Update(engine->time.deltaTime);
}

void Game::Draw()
{
	engine->Get<pbls::Renderer>()->BeginFrame();

	engine->Draw(engine->Get<pbls::Renderer>());
	scene->Draw(engine->Get<pbls::Renderer>());

	engine->Get<pbls::Renderer>()->EndFrame();

}