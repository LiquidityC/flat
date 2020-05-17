#include "UID.h"

namespace flat2d {
	size_t UID::uid = 1;
	SpinLock UID::lock;

	size_t UID::generate()
	{
		lock.lock();
		size_t id = uid++;
		lock.unlock();
		return id;
	}
} // namespace flat2d
