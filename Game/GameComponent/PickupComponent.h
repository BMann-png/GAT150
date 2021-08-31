#pragma once
#include "Component/Component.h"
#include "Framework/EventSystem.h"

class PickupComponent : public pbls::Component
{
public:
	std::unique_ptr<Object> Clone() const { return std::make_unique<PickupComponent>(*this); }

	~PickupComponent();

	void Create() override;
	virtual void Update() override;

	virtual void OnCollisionEnter(const pbls::Event& event);

	virtual bool Write(const rapidjson::Value& value) const override;
	virtual bool Read(const rapidjson::Value& value) override;

public:

};