#include "SpriteAnimationComponent.h"
#include "Engine.h"

namespace pbls
{
	void SpriteAnimationComponent::Update()
	{
		frameTime = 1.0f / fps;

		frameTimer += owner->scene->engine->time.deltaTime;

		if (frameTimer > frameTime)
		{
			frameTimer = 0;
			frame++;
			if (frame > endframe) frame = startframe;
		}

		Vector2 size = texture->GetSize();
		Vector2 frameCount{ numFramesX, numFramesY };
		Vector2 frameSize = size / frameCount;

		rect.x = static_cast<int>((frame % numFramesX) * frameSize.x);
		rect.y = static_cast<int>((frame / numFramesX) * frameSize.y);
		rect.w = static_cast<int>(frameSize.x);
		rect.h = static_cast<int>(frameSize.y);
	}
	void SpriteAnimationComponent::Draw(Renderer* renderer)
	{
		renderer->Draw(texture, rect, owner->transform);
	}
	void SpriteAnimationComponent::StartSequence(const std::string& name)
	{
		if (sequenceName == name) return;

		sequenceName = name;
		if (sequences.find(name) != sequences.end())
		{
			Sequence sequence = sequences[name];
			startframe = sequence.startframe;
			endframe = sequence.endframe;
			fps = sequence.fps;

			frame = startframe;
		}
	}
	bool SpriteAnimationComponent::Write(const rapidjson::Value& value) const
	{
		return false;
	}
	bool SpriteAnimationComponent::Read(const rapidjson::Value& value)
	{
		SpriteComponent::Read(value);

		JSON_READ(value, fps);
		JSON_READ(value, numFramesX);
		JSON_READ(value, numFramesY);
		JSON_READ(value, startframe);
		JSON_READ(value, endframe);

		if (startframe == 0 && endframe == 0) endframe = (numFramesX * numFramesY) - 1;
		frame = startframe;

		if (value.HasMember("sequences") && value["sequences"].IsArray())
		{
			for (auto& sequenceValue : value["sequences"].GetArray())
			{
				std::string name;
				JSON_READ(sequenceValue, name);

				Sequence sequence;
				JSON_READ(sequenceValue, sequence.fps);
				JSON_READ(sequenceValue, sequence.startframe);
				JSON_READ(sequenceValue, sequence.endframe);

				sequences[name] = sequence;
			}

			std::string defaultSequence;
			JSON_READ(value, defaultSequence);
			StartSequence(defaultSequence);
		}

		return false;
	}
}