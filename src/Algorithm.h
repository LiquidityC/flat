#ifndef ALGORITHM_H_
#define ALGORITHM_H_

#include <algorithm>
#include <iostream>

namespace flat2d {
	/**
	 * A class containing nice algorithm utils. This one was originally used
	 * to organize objects in a map by their X,Y positions.
	 * @author Linus Probert <linus.probert@gmail.com>
	 */
	class Algorithm
	{
	  public:
		/**
		 * Creates a unique number from two integers
		 * @param x the first integer
		 * @param y the second integer
		 * @return A unique integer for the two provided integers
		 */
		static int cantorPairing(int x, int y)
		{
			int dx = std::min(x, y);
			int dy = std::max(x, y);

			return ((dx + dy) * (dx + dy + 1)) / 2 + dy;
		}
	};
} // namespace flat2d

#endif // ALGORITHM_H_
