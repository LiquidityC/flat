#ifndef MEDIAUTIL_H_
#define MEDIAUTIL_H_

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>

namespace flat2d {
	/**
	 * The MediaUtil is a helper class to load resource into your game.
	 * You shouldn't have to use this very much any longer. Better to use the
	 * Texture object.
	 * @author Linus Probert <linus.probert@gmail.com>
	 */
	class MediaUtil
	{
	  public:
		/**
		 * Load a texture
		 * @param path The path to the resource
		 * @param renderer The SDL_Renderer
		 * @return an SDL_Texture object
		 */
		static SDL_Texture* loadTexture(const std::string&, SDL_Renderer*);

		/**
		 * Load a ttf font
		 * @param ttfPath The path to the resource
		 * @param size The font size
		 * @return a TTF_Font object
		 */
		static TTF_Font* loadFont(const std::string&, size_t size = 12);

		/**
		 * Create a texture from a TTF_Font and some text
		 * @param text The text to render
		 * @param font the font to use
		 * @param color The text color
		 * @param renderer The renderer
		 * @return an SDL_Texture object
		 */
		static SDL_Texture* createTextTexture(const std::string&,
		                                      TTF_Font*,
		                                      SDL_Color,
		                                      SDL_Renderer*);

		/**
		 * This function performs loadFont and createTextTexture in one quick
		 * step
		 * @param text The text to render
		 * @param ttfPath The path to the font resource
		 * @param renderer The renderer
		 * @return an SDL_Texture object
		 */
		static SDL_Texture* loadTextTexture(const std::string&,
		                                    const std::string&,
		                                    SDL_Renderer*);
	};
} // namespace flat2d

#endif // MEDIAUTIL_H_
