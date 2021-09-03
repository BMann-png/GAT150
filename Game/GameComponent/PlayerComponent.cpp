#include "PlayerComponent.h"
#include "ProjectileComponent.h"
#include "Engine.h"

using namespace pbls;

PlayerComponent::~PlayerComponent()
{
	owner->scene->engine->Get<EventSystem>()->Unsubscribe("collision_enter", owner);
	owner->scene->engine->Get<EventSystem>()->Unsubscribe("collision_exit", owner);
}

void PlayerComponent::Create()
{
	owner->scene->engine->Get<EventSystem>()->Subscribe("collision_enter", std::bind(&PlayerComponent::OnCollisionEnter, this, std::placeholders::_1), owner);
	owner->scene->engine->Get<EventSystem>()->Subscribe("collision_exit", std::bind(&PlayerComponent::OnCollisionExit, this, std::placeholders::_1), owner);

	owner->scene->engine->Get<AudioSystem>()->AddAudio("hurt", "hurt.wav");
}

void PlayerComponent::Update()
{

	Vector2 force = Vector2::zero;
	if (owner->scene->engine->Get<InputSystem>()->GetKeyState(SDL_SCANCODE_A) == InputSystem::eKeyState::Held)
	{
		force.x -= speed;
	}
	if (owner->scene->engine->Get<InputSystem>()->GetKeyState(SDL_SCANCODE_D) == InputSystem::eKeyState::Held)
	{
		force.x += speed;
	}
	if (contacts.size() > 0 && owner->scene->engine->Get<InputSystem>()->GetKeyState(SDL_SCANCODE_SPACE) == InputSystem::eKeyState::Pressed)
	{
		force.y -= jump;
	}

	PhysicsComponent* physicsComponent = owner->GetComponent<PhysicsComponent>();
	assert(physicsComponent);

	physicsComponent->ApplyForce(force);

	SpriteAnimationComponent* spriteAnimationComponent = owner->GetComponent<SpriteAnimationComponent>();
	assert(spriteAnimationComponent);
	if (physicsComponent->velocity.x > 0) spriteAnimationComponent->StartSequence("walk_right");
	else if (physicsComponent->velocity.x < 0) spriteAnimationComponent->StartSequence("walk_left");
	else spriteAnimationComponent->StartSequence("idle");

	//fire
	fireTimer -= owner->scene->engine->time.deltaTime;
	if (fireTimer <= 0 && owner->scene->engine->Get<InputSystem>()->GetButtonState(0) == InputSystem::eKeyState::Pressed)
	{
		fireTimer = fireRate;

		auto Projectile = pbls::ObjectFactory::Instance().Create<pbls::Actor>("Projectile");
		Projectile->transform.position = owner->transform.position;
		Vector2 direction = owner->scene->engine->Get<InputSystem>()->GetMousePosition() - owner->transform.position;
		Projectile->transform.rotation = RadToDeg(direction.Angle());
		Projectile->GetComponent<ProjectileComponent>()->direction = direction.Normalized();
		owner->scene->AddActor(std::move(Projectile));

		std::cout << "FIRE!" << std::endl;
	}
	healthTimer -= owner->scene->engine->time.deltaTime;
	if (health <= 0)
	{
		owner->destroy = true;
	}
}

void PlayerComponent::OnCollisionEnter(const pbls::Event& event)
{
	void* p = std::get<void*>(event.data);
	Actor* actor = reinterpret_cast<Actor*>(p);

	if (istring_compare(actor->tag, "ground"))
	{
		contacts.push_back(actor);
	}

	if (istring_compare(actor->tag, "enemy"))
	{
		owner->scene->engine->Get<AudioSystem>()->PlayAudio("hurt");
		if(healthTimer <= 0) health--;
	}

	std::cout << actor->tag << std::endl;
}

void PlayerComponent::OnCollisionExit(const pbls::Event& event)
{
	void* p = std::get<void*>(event.data);
	Actor* actor = reinterpret_cast<Actor*>(p);

	if (istring_compare(actor->tag, "ground"))
	{
		contacts.remove(actor);
	}
}

bool PlayerComponent::Write(const rapidjson::Value& value) const
{
	return false;
}

bool PlayerComponent::Read(const rapidjson::Value& value)
{
	JSON_READ(value, speed);
	JSON_READ(value, jump);
	JSON_READ(value, fireRate);
	JSON_READ(value, health);

	return true;
}

