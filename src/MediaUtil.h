#ifndef MEDIAUTIL_H_
#define MEDIAUTIL_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>

namespace flat2d
{
	class MediaUtil
	{
		public:
			static SDL_Texture* loadTexture(std::string, SDL_Renderer*);

			static TTF_Font* loadFont(std::string, size_t size = 12);
			static SDL_Texture* createTextTexture(std::string, TTF_Font*, SDL_Color, SDL_Renderer*);
			static SDL_Texture* loadTextTexture(std::string, std::string, SDL_Renderer*);
	};
} // namespace flat2d

#endif // MEDIAUTIL_H_
