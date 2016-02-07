#ifndef TIMER_H_
#define TIMER_H_

namespace flat2d
{
	class Timer
	{
		public:
			Timer();

			void start();
			void stop();
			void pause();
			void unpause();

			Uint32 getTicks() const;

			bool isStarted() const;
			bool isPaused() const;

		private:
			Uint32 startTicks, pausedTicks;

			bool paused, started;
	};
} // namespace flat2d

#endif // TIMER_H_
