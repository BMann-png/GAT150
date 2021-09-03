#pragma once
#include "Component/Component.h"
#include "Framework/EventSystem.h"
#include "Math/Vector2.h"

class ProjectileComponent : public pbls::Component
{
public:
	std::unique_ptr<Object> Clone() const { return std::make_unique<ProjectileComponent>(*this); }

	virtual ~ProjectileComponent();

	void Create() override;
	virtual void Update() override;

	virtual void OnCollision(const pbls::Event& event);

	virtual bool Write(const rapidjson::Value& value) const override;
	virtual bool Read(const rapidjson::Value& value) override;

public:
	float speed{ 0 };
	float lifeTime{ 0 };
	pbls::Vector2 direction = pbls::Vector2::zero;

};
