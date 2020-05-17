#ifndef UID_H_
#define UID_H_

#include "SpinLock.h"
#include <cstddef>

namespace flat2d {
	/**
	 * A Unique id object heavily used by the Entity class
	 * @author Linus Probert <linus.probert@gmail.com>
	 */
	class UID
	{
	  private:
		static size_t uid;
		static SpinLock lock;

	  public:
		/**
		 * Generate a new unique id. These are all sequential and
		 * their life only spans runtime
		 * @return a new uid
		 */
		static size_t generate();
	};
} // namespace flat2d

#endif // UID_H_
