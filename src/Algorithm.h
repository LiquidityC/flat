#ifndef ALGORITHM_H_
#define	ALGORITHM_H_

#include <algorithm>
#include <iostream>

class Algorithm
{
	public:
		/**
		 * Creates a unique number from two integers
		 */
		static int cantorPairing(int x, int y) {
			int dx = std::min(x, y);
			int dy = std::max(x, y);

			return ((dx + dy) * (dx + dy + 1))/2 + dy;
		}
};


#endif // ALGORITHM_H_
