#include "Timer.h"
#include <SDL.h>

namespace flat2d {
	Timer::Timer()
	  : started(false)
	  , paused(false)
	  , startTicks(0)
	  , pausedTicks(0)
	{}

	void Timer::start()
	{
		started = true;
		paused = false;
		startTicks = SDL_GetTicks();
	}

	void Timer::stop()
	{
		started = false;
		paused = false;
		startTicks = 0;
		pausedTicks = 0;
	}

	void Timer::pause()
	{
		if (started && !paused) {
			paused = true;
			pausedTicks = SDL_GetTicks() - startTicks;
			startTicks = 0;
		}
	}

	void Timer::unpause()
	{
		if (started && paused) {
			paused = false;
			startTicks = SDL_GetTicks() - pausedTicks;
			pausedTicks = 0;
		}
	}

	Uint32 Timer::getTicks() const
	{
		if (!started) {
			return 0;
		}

		if (!paused) {
			return SDL_GetTicks() - startTicks;
		} else {
			return pausedTicks;
		}
	}

	bool Timer::isStarted() const { return started; }

	bool Timer::isPaused() const { return paused; }
} // namespace flat2d
