#ifndef CAMERA_H_
#define CAMERA_H_

#include "Square.h"
#include <SDL.h>

namespace flat2d {
	/**
	 * The Camera object that controls what a player can see.
	 * The camera is more or less completely virtual. It has a position and
	 * and can be centered on a position. It's movement is limited by the map
	 * dimensions it's been given. It has a position and can calculate the
	 * "Camera position" for a coordinate for rendering.
	 * @author Linus Probert <linus.probert@gmail.com>
	 */
	class Camera : public Square
	{
	  private:
		int mapWidth, mapHeight;

	  public:
		Camera(int width, int height)
		  : Square(0, 0, width, height)
		  , mapWidth(1920)
		  , mapHeight(1080)
		{}

		virtual ~Camera() {}

		/**
		 * Tell the camera what the "play space" dimensions are.
		 * @param w The space width
		 * @param h The space width
		 */
		void setMapDimensions(int, int);

		/**
		 * Get the play space height
		 * @return the playspace height
		 */
		int getMapHeight();

		/**
		 * Get the play space width
		 * @return the playspace width
		 */
		int getMapWidth();

		/**
		 * Get the camera "box"
		 * @return The camera x, y, width and height as an SDL_Rect
		 */
		SDL_Rect getBox();

		/**
		 * Center the camera on a provided position in playspace.
		 * @param x The X position
		 * @param y The Y position
		 */
		void centerOn(int x, int y);

		/**
		 * Check if a "box" is visible on Camera
		 * @param box The x,y,w,h to check
		 * @param depth The depth of the object (for paralax effects)
		 * @return true or false
		 */
		bool isVisibleOnCamera(const SDL_Rect& box, int depth = 0);

		/**
		 * Check if a "box" is within camera map bounds
		 * @param rect The x,y,w,h to check
		 * @return true or false
		 */
		bool isOutOfMapBounds(const SDL_Rect& rect);

		/**
		 * Get camera x position for provided world x position
		 * @param x The x position to check
		 * @param depth the depth of the x position
		 * @return true or false
		 */
		int getScreenXposFor(int x, int depth = 0) const;

		/**
		 * Get camera y position for provided world y position
		 * @param y The y position to check
		 * @param depth the depth of the y position
		 * @return true or false
		 */
		int getScreenYposFor(int y, int depth = 0) const;
	};
} // namespace flat2d

#endif // CAMERA_H_
