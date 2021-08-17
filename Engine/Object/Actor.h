#pragma once
#include "Object.h"
#include "Math/Transform.h"
#include <vector>
#include <memory>

namespace pbls
{
	class Scene;
	class Texture;
	class Renderer;

	class Actor : public Object
	{
	public:
		Actor() {}
		Actor(const Transform& transform, std::shared_ptr<Texture> texture = {}) : transform{ transform }, texture{ texture }{}

		virtual void Initialize() {}

		virtual void Update(float dt);
		virtual void Draw(Renderer* renderer);

		virtual void OnCollision(Actor* actor) {}
		void AddChild(std::unique_ptr<Actor> actor);

		float GetRadius();

	public:
		bool destroy{ false };
		std::string tag;

		std::shared_ptr<Texture> texture;

		Transform transform;
		Scene* scene{ nullptr };

		Actor* parent{ nullptr };
		std::vector<std::unique_ptr<Actor>> children;
	};
}