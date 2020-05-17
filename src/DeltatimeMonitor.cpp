#include "DeltatimeMonitor.h"
#include <SDL.h>

namespace flat2d {
	void DeltatimeMonitor::updateDeltaTime()
	{
		if (currentTime == 0) {
			currentTime = SDL_GetTicks();
			return;
		}

		oldTime = currentTime;
		currentTime = SDL_GetTicks();
		deltaTime = (currentTime - oldTime) / 1000.0f;
	}

	float DeltatimeMonitor::getDeltaTime() const { return deltaTime; }
} // namespace flat2d
