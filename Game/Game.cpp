#include "Game.h"
#include "GameComponent/PlayerComponent.h"
#include "GameComponent/EnemyComponent.h"
#include "GameComponent/PickupComponent.h"

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


	//create scene
	scene = std::make_unique<pbls::Scene>();
	scene->engine = engine.get();

	pbls::SeedRandom(static_cast<unsigned int>(time(nullptr)));
	pbls::SetFilePath("../Resources");

	rapidjson::Document document;
	pbls::json::Load("scene.txt", document);
	scene->Read(document);


	for (int i = 0; i < 100; i++)
	{
		auto actor = pbls::ObjectFactory::Instance().Create<pbls::Actor>("Coin");
		actor->transform.position = pbls::Vector2{ pbls::RandomRange(0, 800), pbls::RandomRange(100, 400) };
		scene->AddActor(std::move(actor));
	}

	////actors
	//std::unique_ptr<pbls::Actor> actor = std::make_unique<pbls::Actor>(pbls::Transform{ {400, 300} });

	//{
	//	auto component = pbls::ObjectFactory::Instance().Create<pbls::SpriteAnimationComponent>("SpriteAnimationComponent");

	//	//pbls::SpriteAnimationComponent* component = actor->AddComponent<pbls::SpriteAnimationComponent>();
	//	component->texture = engine->Get<pbls::ResourceSystem>()->Get<pbls::Texture>("sparkle.png", engine->Get<pbls::Renderer>());
	//	component->fps = 30;
	//	component->numFramesX = 8;
	//	component->numFramesY = 8;

	//	actor->AddComponent(std::move(component));
	//}
	//{
	//	pbls::PhysicsComponent* component = actor->AddComponent<pbls::PhysicsComponent>();
	//	//component->ApplyForce(pbls::Vector2::right * 200);
	//}

	//scene->AddActor(std::move(actor));

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