#pragma once
#include "box2d/box2d.h"
#include "Object/Actor.h"

namespace pbls
{
	class ContactListener : public b2ContactListener
	{
	public:
		void BeginContact(b2Contact* contact) override;
		void EndContact(b2Contact* contact) override;
	};
}

