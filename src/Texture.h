#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include "Square.h"

namespace flat2d
{
	class Texture : public Square
	{
		private:
			SDL_Texture *texture = nullptr;
			TTF_Font *font = nullptr;

			void freeTexture();
			void freeFont();

		public:
			Texture(int xpos, int ypos) : Square(xpos, ypos, 0, 0) { }
			virtual ~Texture();

			void render(SDL_Renderer *renderer) const;

			bool loadFromFile(std::string path, SDL_Renderer *renderer);
			bool loadFont(std::string path, size_t size);
			bool loadFromRenderedText(std::string text, SDL_Color color, SDL_Renderer *renderer);
	};
} // namespace flat2d

#endif // TEXTURE_H_
