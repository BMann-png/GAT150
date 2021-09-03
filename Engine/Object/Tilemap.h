#pragma once
#include "Object.h"
#include "Core/Serializable.h"
#include "Math/Vector2.h"
#include <vector>

namespace pbls
{
	class Tilemap : public Object, public ISerializable
	{
	public:
		void Create() override;

		virtual bool Write(const rapidjson::Value& value) const override;
		virtual bool Read(const rapidjson::Value& value) override;

	public:
		class Scene* scene = nullptr;

	private:
		int numTilesX{ 0 };
		int numTilesY{ 0 };
		Vector2 start;
		Vector2 size;
		std::vector<std::string> tileNames;
		std::vector<int> tiles;

	};
}