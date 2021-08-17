#include "Scene.h"
#include "Actor.h"
#include <algorithm>

namespace pbls
{
	void pbls::Scene::Update(float dt)
	{
		//add new actors
		actors.insert(actors.end(), std::make_move_iterator(newActors.begin()), std::make_move_iterator(newActors.end()));
		newActors.clear();

		//update actors
		//this uses a lambda function
		std::for_each(actors.begin(), actors.end(), [dt](auto& actor) { actor->Update(dt); });
		//as opposed to the normal for each loop
		//for (auto& actor : actors)
		//{
		//	actor->Update(dt);
		//}

		//collision
		for (size_t i = 0; i < actors.size(); i++)
		{
			for (size_t j = i + 1; j < actors.size(); j++)
			{
				if (actors[i]->destroy || actors[j]->destroy) continue;
				pbls::Vector2 dir = actors[i]->transform.position - actors[j]->transform.position;
				float distance = dir.Length();
				if (distance < actors[i]->GetRadius() + actors[j]->GetRadius())
				{
					actors[i]->OnCollision(actors[j].get());
					actors[j]->OnCollision(actors[i].get());
				}
			}
		}



		//destroy
		auto iter = actors.begin();
		while (iter != actors.end())
		{
			if ((*iter)->destroy)
			{
				iter = actors.erase(iter);
			}
			else
			{
				iter++;
			}

			
		}
	}

	void pbls::Scene::Draw(Renderer* renderer)
	{
		std::for_each(actors.begin(), actors.end(), [renderer](auto& actor) { actor->Draw(renderer); });
	}

	void pbls::Scene::AddActor(std::unique_ptr<Actor> actor)
	{
		actor.get()->scene = this;
		actor->Initialize();

		newActors.push_back(std::move(actor));
	}

	void Scene::RemoveActor(Actor* actor)
	{
	}

	void Scene::RemoveAllActors()
	{
		actors.clear();
	}

}