#pragma once
#include "GraphicsComponent.h"
#include "Math/Color.h"

namespace pbls
{
	class Texture;

	class Font;

	class TextComponent : public GraphicsComponent
	{
	public:
		std::unique_ptr<Object> Clone() const { return std::make_unique<TextComponent>(*this); }

		virtual void Update() override;
		virtual void Draw(Renderer* renderer) override;

		void SetText(const std::string& text);

		virtual bool Write(const rapidjson::Value& value) const override;
		virtual bool Read(const rapidjson::Value& value) override;

	public:
		std::string fontName;
		int fontSize{ 0 };
		Color color{ 0, 0, 0 };
		std::string text;

		std::shared_ptr<Font> font;
		std::shared_ptr<Texture> texture;
	};
}