#ifndef ANIMATION_H_
#define	ANIMATION_H_

#include <SDL.h>
#include <vector>
#include "Timer.h"


namespace flat2d
{
	typedef std::vector<SDL_Rect> Clips;

	/**
	 * An animation to use. Animations are essentially a set of clips (squares(x, y, w, h))
	 * and a timestep. Calling the run function on an Animation will return the current
	 * "clip" to use on your texture.
	 * @author Linus Probert <linus.probert@gmail.com>
	 */
	class Animation
	{
		private:
			Timer animationTimer;

			Clips clips;
			uint32_t timestep;
			int clipIndex = 0;

		public:
			Animation(Clips &c, uint32_t t) : clips(c), timestep(t) { }

			/**
			 * Run the animation and get the current clip
			 * @return the current clip represented as an SDL_Rect
			 */
			const SDL_Rect* run();

			/**
			 * Start the animation
			 */
			void start();

			/**
			 * Stop the animation
			 */
			void stop();

			/**
			 * Check if the animation is running
			 * @return True or false indicating if the animation is running
			 */
			bool isRunning() const;
	};
} // namespace flat2d

#endif // ANIMATION_H_
