#ifndef MAPAREA_H_
#define MAPAREA_H_

#include "Square.h"

namespace flat2d
{
	class MapArea : public Square
	{
		friend class EntityContainer;

		public:
			MapArea(int px, int py, int dim) : Square(px, py, dim) { }
			MapArea(int px, int py, int pw, int ph) : Square(px, py, pw, ph) { }

			SDL_Rect asSDLRect() const {
				return { x, y, w, h };
			}
	};
} // namespace flat2d

#endif // MAPAREA_H_
