#pragma once

#include "Framework/System.h"
#include "Math/Vector2.h"
#include "box2d/box2d.h"
#include "ContactListener.h"

namespace pbls
{
	class PhysicsSystem : public System
	{
	public:
		struct RigidBodyData
		{
			Vector2 size;
			float density{ 0 };
			float friction{ 1 };
			float restitution{ 0.3f };
			float gravityScale{ 1 };
			bool lockAngle{ false };
			bool isDynamic{ false };
			bool isSensor{ false };
		};

	public:
		void Startup() override;
		void Shutdown() override;

		virtual void Update(float dt) override;

		b2Body* CreateBody(const Vector2& position, float angle, const RigidBodyData& data, class Actor* actor = nullptr);
		void DestroyBody(b2Body* body);

		static Vector2 WorldToScreen(const Vector2& world)  { return world * pixelsPerUnit; }
		static Vector2 ScreenToWorld(const Vector2& screen) { return screen * (1.0f / pixelsPerUnit); }

	protected:
		static const float pixelsPerUnit;

		std::unique_ptr<b2World> world;
		std::unique_ptr<ContactListener> contactListener;
	};
}
