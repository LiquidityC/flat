#include "Camera.h"

namespace flat2d
{
	int Camera::getXpos()
	{
		return xpos;
	}

	int Camera::getYpos()
	{
		return ypos;
	}

	int Camera::getWidth()
	{
		return width;
	}

	int Camera::getHeight()
	{
		return height;
	}

	void Camera::setMapDimensions(int w, int h)
	{
		mapWidth = w;
		mapHeight = h;
	}

	int Camera::getMapHeight()
	{
		return mapHeight;
	}

	int Camera::getMapWidth()
	{
		return mapWidth;
	}

	SDL_Rect Camera::getBox()
	{
		SDL_Rect box = { xpos, ypos, width, height };
		return box;
	}

	void Camera::centerOn(int x, int y) {
		xpos = x - (width/2);
		ypos = y - (height/2);

		if (xpos > mapWidth - width) {
			xpos = mapWidth - width;
		} else if (xpos < 0) {
			xpos = 0;
		}
		if (ypos > mapHeight - height) {
			ypos = mapHeight - height;
		} else if (ypos < 0) {
			ypos = 0;
		}
	}

	bool Camera::isVisibleOnCamera(const SDL_Rect& box, int depth)
	{
		int xoffset = depth <= 0 ? 1 : depth * 1.5;
		int yoffset = depth <= 0 ? 1 : depth * 5;

		if (box.x > (xpos / xoffset) + width) {
			return false;
		} else if (box.x + box.w < xpos / xoffset) {
			return false;
		} else if (box.y > (ypos / yoffset) + height) {
			return false;
		} else if (box.y + box.h < ypos / yoffset) {
			return false;
		}
		return true;
	}

	int Camera::getScreenXposFor(int x, int depth) const
	{
		if (depth > 0) {
			return x - (xpos / (1.5 * depth));
		}
		return x - xpos;
	}

	int Camera::getScreenYposFor(int y, int depth) const
	{
		if (depth > 0) {
			return y - (ypos / (5 * depth));
		}
		return y - ypos;
	}
} // namespace flat2d
