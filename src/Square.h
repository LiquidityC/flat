#ifndef SQUARE_H_
#define SQUARE_H_

namespace flat2d
{
	class Square
	{
		protected:
			int x, y, w, h;

		public:
			Square(int px, int py, int dim) : x(px), y(py), w(dim), h(dim) { }
			Square(int px, int py, int pw, int ph) : x(px), y(py), w(pw), h(ph) { }

			bool containsPoint(int, int) const;

			bool operator<(const Square&) const;
			bool operator==(const Square&) const;
			bool operator!=(const Square&) const;

			int getXpos() const { return x; }
			int getYpos() const { return y; }
			int getWidth() const { return w; }
			int getHeight() const { return h; }

			virtual void setXpos(int pos) { x = pos; }
			virtual void setYpos(int pos) { y = pos; }
			virtual void setWidth(int width) { w = width; }
			virtual void setHeight(int height) { h = height; }
	};
} // namespace flat2d
#endif // SQUARE_H_
