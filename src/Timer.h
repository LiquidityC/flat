#ifndef TIMER_H_
#define TIMER_H_

#include <SDL.h>

namespace flat2d {
	/**
	 * A Timer object
	 * @author Linus Probert <linus.probert@gmail.com>
	 */
	class Timer
	{
	  public:
		Timer();

		/**
		 * Start the timer
		 */
		void start();

		/**
		 * Stop the timer
		 */
		void stop();

		/**
		 * Pause the timer
		 */
		void pause();

		/**
		 * Unpause the timer
		 */
		void unpause();

		/**
		 * Get the time passed since start
		 * @return Number of ticks (1000 ticks/s)
		 */
		Uint32 getTicks() const;

		/**
		 * Check if the timer is started
		 * @return true or false
		 */
		bool isStarted() const;

		/**
		 * Check if the timer is paused
		 * @return true or false
		 */
		bool isPaused() const;

	  private:
		Uint32 startTicks, pausedTicks;

		bool paused, started;
	};
} // namespace flat2d

#endif // TIMER_H_
