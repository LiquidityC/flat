#include <SDL_ttf.h>
#include <iostream>
#include <string>

#include "MediaUtil.h"

namespace flat2d {
	SDL_Texture* MediaUtil::loadTexture(const std::string& path,
	                                    SDL_Renderer* renderer)
	{
		SDL_Surface* imgSurface = IMG_Load(path.c_str());
		if (imgSurface == nullptr) {
			std::cerr << "Failed to load image: " << path << std::endl;
			return nullptr;
		}

		SDL_Texture* texture =
		  SDL_CreateTextureFromSurface(renderer, imgSurface);
		if (texture == nullptr) {
			std::cerr << "Unable to create texture from: " << path << std::endl;
		}
		SDL_FreeSurface(imgSurface);

		return texture;
	}

	TTF_Font* MediaUtil::loadFont(const std::string& ttfPath, size_t size)
	{
		TTF_Font* font = TTF_OpenFont(ttfPath.c_str(), size);
		if (font == nullptr) {
			std::cerr << "Failed to load font: " << ttfPath << " : "
			          << TTF_GetError() << std::endl;
			return nullptr;
		}
		return font;
	}

	SDL_Texture* MediaUtil::createTextTexture(const std::string& text,
	                                          TTF_Font* font,
	                                          SDL_Color color,
	                                          SDL_Renderer* renderer)
	{
		SDL_Surface* imgSurface =
		  TTF_RenderText_Solid(font, text.c_str(), color);
		if (imgSurface == nullptr) {
			std::cerr << "Failed to load text image: " << TTF_GetError()
			          << std::endl;
			return nullptr;
		}

		SDL_Texture* texture =
		  SDL_CreateTextureFromSurface(renderer, imgSurface);
		if (texture == nullptr) {
			std::cerr << "Unable to create text texture: " << SDL_GetError()
			          << std::endl;
		}
		SDL_FreeSurface(imgSurface);

		return texture;
	}

	SDL_Texture* MediaUtil::loadTextTexture(const std::string& text,
	                                        const std::string& ttfPath,
	                                        SDL_Renderer* renderer)
	{
		TTF_Font* font = loadFont(ttfPath);
		if (font == nullptr) {
			return nullptr;
		}

		SDL_Color color = { 255, 0, 0 };
		return createTextTexture(text, font, color, renderer);
	}
} // namespace flat2d
