#ifndef UID_H_
#define UID_H_

#include <cstddef>
#include "SpinLock.h"

namespace flat2d
{
	class UID
	{
		private:
			static size_t uid;
			static SpinLock lock;

		public:
			static size_t generate();
	};
} // namespace flat2d

#endif // UID_H_
