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

	bool Camera::isVisibleOnCamera(const SDL_Rect& box)
	{
		if (box.x > xpos + width) {
			return false;
		} else if (box.x + box.w < xpos) {
			return false;
		} else if (box.y > ypos + height) {
			return false;
		} else if (box.y + box.h < ypos) {
			return false;
		}
		return true;
	}

	int Camera::getScreenXposFor(int x) const
	{
		return x - xpos;
	}

	int Camera::getScreenYposFor(int y) const
	{
		return y - ypos;
	}
} // namespace flat2d
