#ifndef RENDERDATA_H_
#define RENDERDATA_H_

#include <SDL.h>

namespace flat2d {
	class Camera;
	class EntityContainer;
	class CollisionDetector;

	/**
	 * The RenderData object acts as a container for everything related
	 * to what the player sees in game. From it you can get the SDL_Renderer,
	 * Camera
	 *
	 * @author Linus Probert <linus.probert@gmail.com>
	 */
	class RenderData
	{
	  private:
		SDL_Renderer* renderer;
		Camera* camera;

	  public:
		RenderData(SDL_Renderer* ren, Camera* cam)
		  : renderer(ren)
		  , camera(cam)
		{}

		/**
		 * Get the SDL_Renderer
		 * @return The SDL_Renderer object pointer
		 */
		SDL_Renderer* getRenderer() const { return renderer; }

		/**
		 * Get the Camera
		 * @return The Camera object pointer
		 */
		Camera* getCamera() const { return camera; }
	};
} // namespace flat2d

#endif // RENDERDATA_H_
