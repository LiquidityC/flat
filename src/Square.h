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
	};
} // namespace flat2d
#endif // SQUARE_H_
