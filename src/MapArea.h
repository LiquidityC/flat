#ifndef MAPAREA_H_
#define MAPAREA_H_

#include "EntityShape.h"
#include "Square.h"

namespace flat2d {
	/**
	 * The MapArea represents and area of the game space. It
	 * is used mainly for spatial partitioning.
	 * @author Linus Probert <linus.probert@gmail.com>
	 */
	class MapArea : public Square
	{
		friend class EntityContainer;

	  public:
		MapArea(int px, int py, int dim)
		  : Square(px, py, dim)
		{}
		MapArea(int px, int py, int pw, int ph)
		  : Square(px, py, pw, ph)
		{}

		/**
		 * Return the MapArea as an SDL_Rect
		 * @return an SDL_Rect
		 */
		SDL_Rect asSDLRect() const { return { x, y, w, h }; }

		EntityShape asEntityShape() const { return { x, y, w, h }; }
	};
} // namespace flat2d

#endif // MAPAREA_H_
