#include "Player.h"
#include "Math/MathUtils.h"
#include "Projectile.h"
#include "Enemy.h"
#include <memory>

Player::Player(const pbls::Transform& transform, std::shared_ptr<pbls::Texture> texture, float speed) : pbls::Actor{ transform, texture }, speed{ speed }
{
	

}

void Player::Initialize()
{
	invincibilityTimer = invincibilityTimerSet;

	std::unique_ptr locator = std::make_unique<Actor>();
	locator->transform.localPosition = pbls::Vector2{ 9,0 };
	AddChild(std::move(locator));

	locator = std::make_unique<Actor>();
	locator->transform.localPosition = pbls::Vector2{ 8,5 };
	AddChild(std::move(locator));

	locator = std::make_unique<Actor>();
	locator->transform.localPosition = pbls::Vector2{ 8,-5 };
	AddChild(std::move(locator));

	locator = std::make_unique<Actor>();
	locator->transform.localPosition = pbls::Vector2{ -20, 0 };
	locator->transform.localRotation = pbls::DegToRad(180);
	AddChild(std::move(locator));
}

void Player::Update(float dt)
{
	if (invincibilityTimer > 0)
	{
		invincibilityTimer -= dt;
	}

	float thrust = 0;
	if (scene->engine->Get<pbls::InputSystem>()->GetKeyState(SDL_SCANCODE_A) == pbls::InputSystem::eKeyState::Held) { transform.rotation -= 5 * dt; }
	if (scene->engine->Get<pbls::InputSystem>()->GetKeyState(SDL_SCANCODE_D) == pbls::InputSystem::eKeyState::Held) { transform.rotation += 5 * dt; }
	if (scene->engine->Get<pbls::InputSystem>()->GetKeyState(SDL_SCANCODE_W) == pbls::InputSystem::eKeyState::Held) { thrust = speed; }
	//if (Core::Input::IsPressed('S')) { input += pbls::Vector2::down; }

	pbls::Vector2 acceleration;
	acceleration += pbls::Vector2::Rotate(pbls::Vector2::right, transform.rotation) * thrust;
	pbls::Vector2 gravity = (pbls::Vector2{ 400, 300 } - transform.position).Normalized() * 30;
	acceleration += gravity;

	velocity += acceleration * dt;

	transform.position += velocity * dt;

	velocity *= 0.98f;

	transform.position.x = pbls::Wrap(transform.position.x, 0.0f, 800.0f);
	transform.position.y = pbls::Wrap(transform.position.y, 0.0f, 600.0f);

	if (shotsFire % 10 == 1)
	{
		shotsFire++;
		bombs++;
	}

	//fire
	fireTimer -= dt;
	if (scene->engine->Get<pbls::InputSystem>()->GetKeyState(SDL_SCANCODE_V) == pbls::InputSystem::eKeyState::Pressed)
	{
		if (weaponType == 0) { weaponType = 1;}
		else { weaponType = 0; }
	}
	if (weaponType == 0)
	{
		fireRate = 0.3f;
		if (fireTimer <= 0 && scene->engine->Get<pbls::InputSystem>()->GetKeyState(SDL_SCANCODE_SPACE) == pbls::InputSystem::eKeyState::Held)
		{
			shotsFire++;

			fireTimer = fireRate;
			{
				pbls::Transform t = children[1]->transform;
				t.scale = 0.1f;

				std::unique_ptr<Projectile> projectile = (std::make_unique<Projectile>(t, scene->engine->Get<pbls::ResourceSystem>()->Get<pbls::Texture>("Textures/boolet.png", scene->engine->Get<pbls::Renderer>()), 600));
				projectile->tag = "Player";
				scene->AddActor(std::move(projectile));
			}
			{
				pbls::Transform t = children[2]->transform;
				t.scale = 0.1f;

				std::unique_ptr<Projectile> projectile = (std::make_unique<Projectile>(t, scene->engine->Get<pbls::ResourceSystem>()->Get<pbls::Texture>("Textures/boolet.png", scene->engine->Get<pbls::Renderer>()), 600));
				projectile->tag = "Player";
				scene->AddActor(std::move(projectile));
			}
			scene->engine->Get<pbls::AudioSystem>()->PlayAudio("playerFire");

		}
	}
	if (weaponType == 1)
	{
		fireRate = 0.2f;
		if (fireTimer <= 0 && scene->engine->Get<pbls::InputSystem>()->GetKeyState(SDL_SCANCODE_SPACE) == pbls::InputSystem::eKeyState::Held)
		{
			shotsFire++;

			fireTimer = fireRate;
			{
				pbls::Transform t = children[0]->transform;
				t.scale = 0.15f;

				std::unique_ptr<Projectile> projectile = (std::make_unique<Projectile>(t, scene->engine->Get<pbls::ResourceSystem>()->Get<pbls::Texture>("Textures/boolet.png", scene->engine->Get<pbls::Renderer>()), 600));
				projectile->tag = "Player";
				scene->AddActor(std::move(projectile));
			}
			scene->engine->Get<pbls::AudioSystem>()->PlayAudio("playerFire");

		}
	}
	if (fireTimer <= 0 && scene->engine->Get<pbls::InputSystem>()->GetKeyState(SDL_SCANCODE_C) == pbls::InputSystem::eKeyState::Pressed && bombs > 0)
	{
		bombs--;
		fireTimer = fireRate;

		pbls::Transform t = children[0]->transform;
		t.scale = 0.2f;

		for (int i = 0; i < 12; i++)
		{
			t.rotation += pbls::DegToRad(i * 30);
			std::unique_ptr<Projectile> projectile = (std::make_unique<Projectile>(t, scene->engine->Get<pbls::ResourceSystem>()->Get<pbls::Texture>("Textures/boolet.png", scene->engine->Get<pbls::Renderer>()), 600));
			projectile->tag = "Player";
			scene->AddActor(std::move(projectile));
		}
	}

	std::vector<std::shared_ptr<pbls::Texture>> smoke = { scene->engine->Get<pbls::ResourceSystem>()->Get<pbls::Texture>("explosion1", scene->engine->Get<pbls::Renderer>()), scene->engine->Get<pbls::ResourceSystem>()->Get<pbls::Texture>("explosion2", scene->engine->Get<pbls::Renderer>()) };
	scene->engine->Get<pbls::ParticleSystem>()->Create(children[3]->transform.position, 3, 0.5f, smoke, 40, children[3]->transform.rotation, pbls::DegToRad(15));

	Actor::Update(dt);
}

void Player::OnCollision(Actor* actor)
{

	if (dynamic_cast<Enemy*>(actor) && invincibilityTimer <= 0)
	{
		PlayerHit();
	}
	if (dynamic_cast<Projectile*>(actor) && actor->tag == "Enemy" && invincibilityTimer <= 0)
	{
		PlayerHit();
	}
	
}

void Player::PlayerHit()
{
	//scene->engine->Get<pbls::ParticleSystem>()->Create(transform.position, 50, 2, pbls::Color::red, 40);
	scene->engine->Get<pbls::AudioSystem>()->PlayAudio("explosion");

	pbls::Event event;
	event.name = "PlayerHit";
	event.data = std::string("yes I'm dead!");
	scene->engine->Get<pbls::EventSystem>()->Notify(event);

	invincibilityTimer = invincibilityTimerSet;
}
