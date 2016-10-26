#ifndef ANIMATION_H_
#define	ANIMATION_H_

#include <SDL2/SDL.h>
#include <vector>
#include "Timer.h"


namespace flat2d
{
	typedef std::vector<SDL_Rect> Clips;

	class Animation
	{
		private:
			Timer animationTimer;

			Clips clips;
			uint32_t timestep;
			int clipIndex = 0;

		public:
			Animation(Clips c, uint32_t t) : clips(c), timestep(t) { }

			const SDL_Rect* run();
			void start();
			void stop();
			bool isRunning() const;
	};
} // namespace flat2d

#endif // ANIMATION_H_
