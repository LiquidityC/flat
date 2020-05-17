#ifndef SPINLOCK_H_
#define SPINLOCK_H_

#include <atomic>

namespace flat2d {
	/**
	 * A spinlock implementation: https://en.wikipedia.org/wiki/Spinlock
	 * @author Linus Probert <linus.probert@gmail.com>
	 */
	class SpinLock
	{
	  private:
		std::atomic_flag lck = ATOMIC_FLAG_INIT;

	  public:
		/**
		 * Lock the spinlock
		 */
		void lock()
		{
			while (lck.test_and_set(std::memory_order_acquire)) {
			}
		}

		/**
		 * Unlock the spinlock
		 */
		void unlock() { lck.clear(std::memory_order_release); }
	};
} // namespace flat2d

#endif // SPINLOCK_H_
