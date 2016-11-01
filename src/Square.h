#ifndef SQUARE_H_
#define SQUARE_H_

namespace flat2d
{
	/**
	 * An object that represents a square
	 * @author Linus Probert <linus.probert@gmail.com>
	 */
	class Square
	{
		protected:
			int x, y, w, h;

		public:
			Square() : x(0), y(0), w(0), h(0) { }
			Square(int px, int py, int dim) : x(px), y(py), w(dim), h(dim) { }
			Square(int px, int py, int pw, int ph) : x(px), y(py), w(pw), h(ph) { }

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
			 * Get the Square width
			 * @return an integer
			 */
			int getWidth() const { return w; }

			/**
			 * Get the Square height
			 * @return an integer
			 */
			int getHeight() const { return h; }

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

			/**
			 * Set the Square width
			 * @param width The new width
			 */
			virtual void setWidth(int width) { w = width; }

			/**
			 * Set the Square height
			 * @param height The new height
			 */
			virtual void setHeight(int height) { h = height; }
	};
} // namespace flat2d
#endif // SQUARE_H_
