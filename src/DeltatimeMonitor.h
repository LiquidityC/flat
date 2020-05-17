#ifndef DELTATIMEMONITOR_H_
#define DELTATIMEMONITOR_H_

namespace flat2d {
	/**
	 * The DeltatimeMonitor keeps track of deltatimes during engine loops
	 * @author Linus Probert <linus.probert@gmail.com>
	 */
	class DeltatimeMonitor
	{
	  private:
		float deltaTime = 1.0;
		int currentTime = 0;
		int oldTime = 0;

	  public:
		/**
		 * Update the deltatime. This is used by the GameEngine and
		 * it's most likely a bad idea calling this from game code.
		 */
		void updateDeltaTime();

		/**
		 * Get the current deltatime. You can use this to correctly move
		 * your objects during update steps.
		 */
		float getDeltaTime() const;
	};
} // namespace flat2d

#endif // DELTATIMEMONITOR_H_
