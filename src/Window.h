#ifndef WINDOW_H_
#define WINDOW_H_

#include <SDL.h>
#include <cstdio>
#include <string>

namespace flat2d {
	/**
	 * The window class. A game can run multiple windows
	 * @author Linus Probert <linus.probert@gmail.com>
	 */
	class Window
	{
	  private:
		std::string title;
		unsigned int width, height;

		SDL_Window* window = nullptr;
		SDL_Renderer* renderer = nullptr;

	  public:
		Window(std::string& t, unsigned int w, unsigned int h)
		  : title(t)
		  , width(w)
		  , height(h)
		{}

		~Window()
		{
			if (window) {
				SDL_DestroyWindow(window);
				window = nullptr;
			}
			if (renderer) {
				SDL_DestroyRenderer(renderer);
				renderer = nullptr;
			}
		}

		/**
		 * Initiate the window
		 */
		bool init();

		/**
		 * Get the SDL_Window from Window
		 * @return SDL_Window
		 */
		SDL_Window* getWindow() { return window; }

		/**
		 * Get the SDL_Renderer from Window
		 * @return SDL_Renderer
		 */
		SDL_Renderer* getRenderer() { return renderer; }
	};
} // namespace flat2d

#endif // WINDOW_H_
