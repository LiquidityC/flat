#ifndef SQUARE_H_
#define SQUARE_H_

#include "Dimension.h"

namespace flat2d {
	/**
	 * An object that represents a square
	 * @author Linus Probert <linus.probert@gmail.com>
	 */
	class Square : public Dimension
	{
	  protected:
		int x, y;

	  public:
		Square()
		  : Dimension(0, 0)
		  , x(0)
		  , y(0)
		{}
		Square(int px, int py, int dim)
		  : Dimension(dim, dim)
		  , x(px)
		  , y(py)
		{}
		Square(int px, int py, int pw, int ph)
		  : Dimension(pw, ph)
		  , x(px)
		  , y(py)
		{}

		/**
		 * Check if the Square contains a given point
		 * @param px The x position
		 * @param py The y position
		 * @return true or false
		 */
		bool containsPoint(int, int) const;

		bool operator<(const Square&) const;
		bool operator==(const Square&) const;
		bool operator!=(const Square&) const;

		/**
		 * Get the Square X position
		 * @return an integer
		 */
		int getXpos() const { return x; }

		/**
		 * Get the Square Y position
		 * @return an integer
		 */
		int getYpos() const { return y; }

		/**
		 * Set the Square X position
		 * @param pos The new position
		 */
		virtual void setXpos(int pos) { x = pos; }

		/**
		 * Set the Square Y position
		 * @param pos The new position
		 */
		virtual void setYpos(int pos) { y = pos; }
	};
} // namespace flat2d
#endif // SQUARE_H_
