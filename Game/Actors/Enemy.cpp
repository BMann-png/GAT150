#include "Enemy.h"
#include "Math/MathUtils.h"
#include "Projectile.h"
#include "Engine.h"
#include "Player.h"

void Enemy::Update(float dt)
{

	if (scene->GetActor<Player>())
	{
		pbls::Vector2 direction = scene->GetActor<Player>()->transform.position - transform.position;
		pbls::Vector2 forward = pbls::Vector2::Rotate(pbls::Vector2::right, transform.rotation);

		float turnAngle = pbls::Vector2::SignedAngle(forward, direction.Normalized());
		//		transform.rotation = transform.rotation + turnAngle + (2 * dt);
		transform.rotation = pbls::Lerp(transform.rotation, transform.rotation + turnAngle, 2 * dt);

		float angle = pbls::Vector2::Angle(forward, direction.Normalized());

		//fire
		fireTimer -= dt;
		if (fireTimer <= 0 && angle <= pbls::DegToRad(10))
		{
			fireTimer = fireRate;

			pbls::Transform t = transform;
			t.scale = 0.05f;

			std::unique_ptr<Projectile> projectile = (std::make_unique<Projectile>(t, scene->engine->Get<pbls::ResourceSystem>()->Get<pbls::Texture>("Textures/boolet.png", scene->engine->Get<pbls::Renderer>()), 350));
			projectile->tag = "Enemy";

			scene->AddActor(std::move(projectile));

			scene->engine->Get<pbls::AudioSystem>()->PlayAudio("enemyFire");
		}
	}

	transform.position += pbls::Vector2::Rotate(pbls::Vector2::right, transform.rotation) * speed * dt;
	transform.position.x = pbls::Wrap(transform.position.x, 0.0f, 800.0f);
	transform.position.y = pbls::Wrap(transform.position.y, 0.0f, 600.0f);

	Actor::Update(dt);
}

void Enemy::OnCollision(Actor* actor)
{
	if (dynamic_cast<Projectile*>(actor) && actor->tag == "Player")
	{
		destroy = true;
		//scene->engine->Get<pbls::ParticleSystem>()->Create(transform.position, 50, 2, pbls::Color::red, 40);
		scene->engine->Get<pbls::AudioSystem>()->PlayAudio("explosion");

		pbls::Event event;
		event.name = "AddPoints";
		event.data = 300;
		scene->engine->Get<pbls::EventSystem>()->Notify(event);
	}
}
