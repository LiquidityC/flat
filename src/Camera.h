#ifndef CAMERA_H_
#define CAMERA_H_

#include <SDL2/SDL.h>

namespace flat2d
{
	class Camera
	{
		private:
			int xpos, ypos, width, height, mapWidth, mapHeight;

		public:
			Camera(int w, int h) :
				xpos(0),
				ypos(0),
				width(w),
				height(h),
				mapWidth(1920),
				mapHeight(1080) { }

			int getXpos();
			int getYpos();
			int getWidth();
			int getHeight();

			void setMapDimensions(int, int);
			int getMapHeight();
			int getMapWidth();

			SDL_Rect getBox();

			void centerOn(int x, int y);

			bool isVisibleOnCamera(const SDL_Rect& box, int depth = 0);
			bool isOutOfMapBounds(const SDL_Rect& rect);

			int getScreenXposFor(int x, int depth = 0) const;
			int getScreenYposFor(int y, int depth = 0) const;
	};
} // namespace flat2d

#endif // CAMERA_H_
