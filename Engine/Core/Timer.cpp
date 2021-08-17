#include "Timer.h"

namespace pbls
{
	Timer::clock_tick_type Timer::ElapsedTicks()
	{
		clock_duration duration = clock::now() - timePoint;
		return clock_tick_type();
	}
	float Timer::ElapsedSeconds()
	{
		return ElapsedTicks() / static_cast<float>(clock_duration::period::den);
	}
	void FrameTimer::Tick()
	{
		clock_duration duration = clock::now() - startTimePoint;
		time = duration.count() / static_cast<float>(clock_duration::period::den);

		duration = clock::now() - frameTimePoint;
		deltaTime = (duration.count() / static_cast<float>(clock_duration::period::den)) * timeScale;

		frameTimePoint = clock::now();
	}
}