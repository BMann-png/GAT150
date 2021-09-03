#include "ProjectileComponent.h"
#include "Engine.h"

using namespace pbls;

ProjectileComponent::~ProjectileComponent()
{
	owner->scene->engine->Get<EventSystem>()->Unsubscribe("collision_enter", owner);
}

void ProjectileComponent::Create()
{
	owner->scene->engine->Get<EventSystem>()->Subscribe("collision_enter", std::bind(&ProjectileComponent::OnCollision, this, std::placeholders::_1), owner);
}

void ProjectileComponent::Update()
{
	PhysicsComponent* physicsComponent = owner->GetComponent<PhysicsComponent>();
	assert(physicsComponent);

	physicsComponent->ApplyForce(direction * speed);
}

void ProjectileComponent::OnCollision(const pbls::Event& event)
{
	void* p = std::get<void*>(event.data);
	Actor* actor = reinterpret_cast<Actor*>(p);

	if (istring_compare(actor->tag, "ground"))
	{
		owner->destroy = true;
	}
}


bool ProjectileComponent::Write(const rapidjson::Value& value) const
{
	return false;
}

bool ProjectileComponent::Read(const rapidjson::Value& value)
{
	JSON_READ(value, speed);

	return true;
}
