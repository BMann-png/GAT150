#pragma once
#include "Object/Actor.h"

class Player : public pbls::Actor
{
public:
	Player(const pbls::Transform& transform, std::shared_ptr<pbls::Texture> texture, float speed);

	void Initialize() override;

	void Update(float dt) override;
	void OnCollision(Actor* actor) override;
	void PlayerHit();

	size_t getBombs() { return bombs; }

private:
	float fireTimer{ 0 };
	float fireRate{ 0.3f };
	float speed{ 30 };
	float invincibilityTimer{ 0 };
	float invincibilityTimerSet{ 2 };

	size_t bombs{ 0 };
	size_t shotsFire{ 0 };
	size_t weaponType{ 0 };

	pbls::Vector2 velocity;
};