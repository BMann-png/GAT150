#pragma once
#include "Object/Object.h"
#include "Core/Serializable.h"

namespace pbls
{
	class Actor;

	class Component : public Object, public ISerializable
	{
	public:
		virtual void Update() = 0;

	public:
		Actor* owner{ nullptr };
	};
}