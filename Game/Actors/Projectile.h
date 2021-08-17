#pragma once
#include "Object/Actor.h"
#include "Engine.h"

class Projectile : public pbls::Actor
{
public:
	Projectile(const pbls::Transform& transform, std::shared_ptr<pbls::Texture> texture, float speed) : pbls::Actor{ transform, texture }, speed{ speed } {}


	void Update(float dt) override;
	void OnCollision(Actor* actor) override;

private:
	float lifetime{ 1.0f };
	float speed{ 20 };
};