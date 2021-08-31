#include "AudioComponent.h"
#include "Engine.h"

namespace pbls
{

void AudioComponent::Update()
{
	if (playOnAwake && !played)
	{ 
		played = true;
		Play(); 
	}
}

void AudioComponent::Play()
{
	channel.Stop();
	channel = owner->scene->engine->Get<AudioSystem>()->PlayAudio(soundName, volume, pitch, isLoop);
}

void AudioComponent::Stop()
{
	channel.Stop();
}


bool AudioComponent::Write(const rapidjson::Value& value) const
{
	return false;
}

bool AudioComponent::Read(const rapidjson::Value& value)
{
	JSON_READ(value, soundName);
	JSON_READ(value, volume);
	JSON_READ(value, pitch);
	JSON_READ(value, isLoop);
	JSON_READ(value, playOnAwake);

	if (soundName != "") owner->scene->engine->Get<AudioSystem>()->AddAudio(soundName, soundName);

	return true;
}
}
