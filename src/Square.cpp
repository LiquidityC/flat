#include "Square.h"

namespace flat2d {
	bool Square::containsPoint(int px, int py) const
	{
		return px >= x && px <= x + w && py >= y && py <= y + h;
	}

	bool Square::operator<(const Square& s) const
	{
		if (x == s.x) {
			return y < s.y;
		} else {
			return x < s.x;
		}
	}

	bool Square::operator==(const Square& s) const
	{
		return w == s.w && h == s.h && !(*this < s) && !(s < *this);
	}

	bool Square::operator!=(const Square& s) const { return !(*this == s); }
} // namespace flat2d
