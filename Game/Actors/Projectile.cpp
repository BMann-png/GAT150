#include "Projectile.h"
#include "Math/MathUtils.h"
#include "Engine.h"
#include "Enemy.h"
#include "Player.h"

void Projectile::Update(float dt)
{
	lifetime -= dt;
	destroy =  (lifetime <= 0);
	
	transform.position += pbls::Vector2::Rotate(pbls::Vector2::right, transform.rotation) * speed * dt;

	if (tag == "Player")
	{
		transform.position.x = pbls::Wrap(transform.position.x, 0.0f, 800.0f);
		transform.position.y = pbls::Wrap(transform.position.y, 0.0f, 600.0f);
	}
	//scene->engine->Get<pbls::ParticleSystem>()->Create(transform.position, 3, 0.5f, scene->engine->Get<pbls::ResourceSystem>()->Get<pbls::Texture>("explosion1", scene->engine->Get<pbls::Renderer>()), 10);
	
	Actor::Update(dt);
}

void Projectile::OnCollision(Actor* actor)
{
	if ((dynamic_cast<Enemy*>(actor) && tag == "Player") || (dynamic_cast<Player*>(actor) && tag == "Enemy"))
	{
		destroy = true;
	}
}
