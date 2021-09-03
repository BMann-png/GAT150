#include "PickupComponent.h"
#include "Engine.h"

using namespace pbls;

PickupComponent::~PickupComponent()
{
	owner->scene->engine->Get<EventSystem>()->Unsubscribe("collision_enter", owner);

}

void PickupComponent::Create()
{
	owner->scene->engine->Get<EventSystem>()->Subscribe("collision_enter", std::bind(&PickupComponent::OnCollisionEnter, this, std::placeholders::_1), owner);
	
	owner->scene->engine->Get<AudioSystem>()->AddAudio("coin", "coin.wav");
}

void PickupComponent::Update()
{
}

void PickupComponent::OnCollisionEnter(const pbls::Event& event)
{
	void* p = std::get<void*>(event.data);
	Actor* actor = reinterpret_cast<Actor*>(p);

	if (istring_compare(actor->tag, "Player"))
	{
		owner->destroy = true;
		owner->scene->engine->Get<AudioSystem>()->PlayAudio("coin");

		Event event;
		event.name = "onAddScore";
		event.data = 10;

		owner->scene->engine->Get<EventSystem>()->Notify(event);
	}
}

bool PickupComponent::Write(const rapidjson::Value& value) const
{
	return false;
}

bool PickupComponent::Read(const rapidjson::Value& value)
{
	return true;
}
