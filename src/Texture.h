#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include "Square.h"

namespace flat2d
{
	/**
	 * A Texture object
	 * @author Linus Probert <linus.probert@gmail.com>
	 */
	class Texture : public Square
	{
		private:
			SDL_Texture *texture = nullptr;
			TTF_Font *font = nullptr;

			void freeTexture();
			void freeFont();

		public:
			Texture() : Square() { }
			Texture(int x, int y) : Square(x, y, 0, 0) { }
			explicit Texture(SDL_Texture* t) : Square(x, y, 0, 0), texture(t) { }
			virtual ~Texture();

			/**
			 * Render the texture
			 * @param renderer The SDL_Renderer
			 * @param clip The texture clip to render
			 * @param pos Position and dimension to render at
			 */
			void render(SDL_Renderer *renderer,
					const SDL_Rect* clip = nullptr,
					const SDL_Rect* pos = nullptr
					) const;

			/**
			 * Load a texture resource from file
			 * @param path The resource path
			 * @param renderer The SDL_Renderer
			 * @return success or fail
			 */
			bool loadFromFile(std::string path, SDL_Renderer *renderer);

			/**
			 * Load a font resource into the Texture
			 * @param path The resource path
			 * @param size The font size
			 * @return success or fail
			 */
			bool loadFont(std::string path, size_t size);

			/**
			 * Create a texture from the loaded font
			 * @param text The message to render
			 * @param color The color to render font with
			 * @param renderer The SDL_Renderer
			 */
			bool loadFromRenderedText(std::string text, SDL_Color color, SDL_Renderer *renderer);
	};
} // namespace flat2d

#endif // TEXTURE_H_
