#include "EnemyComponent.h"
#include "Engine.h"

using namespace pbls;

EnemyComponent::~EnemyComponent()
{
	owner->scene->engine->Get<EventSystem>()->Unsubscribe("collision_enter", owner);
}

void EnemyComponent::Create()
{
	owner->scene->engine->Get<EventSystem>()->Subscribe("collision_enter", std::bind(&EnemyComponent::OnCollision, this, std::placeholders::_1), owner);
}

void EnemyComponent::Update()
{
	Actor* player = owner->scene->FindActor("Player");
	if (player)
	{
		Vector2 direction = player->transform.position - owner->transform.position;
		Vector2 force = direction.Normalized() * speed;

		PhysicsComponent* physicsComponent = owner->GetComponent<PhysicsComponent>();
		assert(physicsComponent);

		physicsComponent->ApplyForce(force);
	}
}

void EnemyComponent::OnCollision(const pbls::Event& event)
{
	void* p = std::get<void*>(event.data);
	Actor* actor = reinterpret_cast<Actor*>(p);

	if (istring_compare(actor->tag, "Player"))
	{
		owner->destroy = true;
		owner->scene->engine->Get<AudioSystem>()->PlayAudio("coin");

		Event event;
		event.name = "onAddScore";
		event.data = 30;

		owner->scene->engine->Get<EventSystem>()->Notify(event);
	}
}


bool EnemyComponent::Write(const rapidjson::Value& value) const
{
	return false;
}

bool EnemyComponent::Read(const rapidjson::Value& value)
{
	JSON_READ(value, speed);

	return true;
}
