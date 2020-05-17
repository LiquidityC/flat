#include "Camera.h"

namespace flat2d {
	void Camera::setMapDimensions(int w, int h)
	{
		mapWidth = w;
		mapHeight = h;
	}

	int Camera::getMapHeight() { return mapHeight; }

	int Camera::getMapWidth() { return mapWidth; }

	SDL_Rect Camera::getBox()
	{
		SDL_Rect box = { x, y, w, h };
		return box;
	}

	void Camera::centerOn(int xpos, int ypos)
	{
		x = xpos - (w / 2);
		y = ypos - (h / 2);

		if (x > mapWidth - w) {
			x = mapWidth - w;
		} else if (x < 0) {
			x = 0;
		}
		if (y > mapHeight - h) {
			y = mapHeight - h;
		} else if (y < 0) {
			y = 0;
		}
	}

	bool Camera::isVisibleOnCamera(const SDL_Rect& box, int depth)
	{
		int bx = getScreenXposFor(box.x, depth);
		int by = getScreenYposFor(box.y, depth);

		if (bx > w) {
			return false;
		} else if (bx + box.w < 0) {
			return false;
		} else if (by > h) {
			return false;
		} else if (by + box.h < 0) {
			return false;
		}
		return true;
	}

	bool Camera::isOutOfMapBounds(const SDL_Rect& rect)
	{
		return rect.x + rect.w < 0 || rect.y + rect.h < 0 ||
		       rect.y > mapHeight || rect.x > mapWidth;
	}

	int Camera::getScreenXposFor(int xpos, int depth) const
	{
		if (depth > 0) {
			return xpos - (int)(x / (1.5 * depth));
		}
		return xpos - x;
	}

	int Camera::getScreenYposFor(int ypos, int depth) const
	{
		if (depth > 0) {
			return ypos - (y / (5 * depth));
		}
		return ypos - y;
	}
} // namespace flat2d
