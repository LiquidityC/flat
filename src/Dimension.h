#ifndef DIMENSION_H_
#define DIMENSION_H_

namespace flat2d {
	/**
	 * An object representing a dimension with width and height.
	 * @author Linus Probert <linus.probert@gmail.com>
	 */
	class Dimension
	{
	  protected:
		int w = 0, h = 0;

	  public:
		explicit Dimension(int width, int height)
		  : w(width)
		  , h(height)
		{}

		/**
		 * Get the Dimension width
		 * @return the width
		 */
		int getWidth() const { return w; }

		/**
		 * Get the Dimension height
		 * @return the height
		 */
		int getHeight() const { return h; }

		/**
		 * Set the Dimension width
		 * @param width The new width
		 */
		void setWidth(int width) { w = width; }

		/**
		 * Set the Dimension height
		 * @param height The new height
		 */
		void setHeight(int height) { h = height; }
	};
} // namespace flat2d

#endif // DIMENSION_H_
