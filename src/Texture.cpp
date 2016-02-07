#include <iostream>
#include <string>
#include "Texture.h"
#include "MediaUtil.h"

namespace flat2d
{
	Texture::~Texture()
	{
		freeFont();
		freeTexture();
	}

	void Texture::freeFont()
	{
		if (font != nullptr) {
			TTF_CloseFont(font);
			font = nullptr;
		}
	}

	void Texture::freeTexture()
	{
		if (texture != nullptr) {
			SDL_DestroyTexture(texture);
			texture = nullptr;
		}
	}

	bool Texture::loadFromFile(std::string path, SDL_Renderer *renderer)
	{
		freeTexture();

		SDL_Surface* imgSurface = IMG_Load(path.c_str());
		if (imgSurface == nullptr) {
			std::cerr << "Failed to load image: " << path << std::endl;
			return false;
		}

		texture = SDL_CreateTextureFromSurface(renderer, imgSurface);
		if (texture == nullptr) {
			std::cerr << "Unable to create texture from: " << path << std::endl;
		}

		this->w = imgSurface->w;
		this->h = imgSurface->h;

		SDL_FreeSurface(imgSurface);

		return texture != nullptr;
	}

	bool Texture::loadFont(std::string path, size_t size)
	{
		freeFont();
		font = MediaUtil::loadFont(path, size);
		return font != nullptr;
	}

	bool Texture::loadFromRenderedText(std::string text, SDL_Color color, SDL_Renderer *renderer)
	{
		freeTexture();

		SDL_Surface* imgSurface = TTF_RenderText_Solid(font, text.c_str(), color);
		if (imgSurface == nullptr) {
			std::cerr << "Failed to load text image: " << TTF_GetError() << std::endl;
			return false;
		}

		texture = SDL_CreateTextureFromSurface(renderer, imgSurface);
		if (texture == nullptr) {
			std::cerr << "Unable to create text texture: " << SDL_GetError() << std::endl;
		}

		this->w = imgSurface->w;
		this->h = imgSurface->h;

		SDL_FreeSurface(imgSurface);

		return texture != nullptr;
	}

	void Texture::render(SDL_Renderer *renderer) const
	{
		if (texture != nullptr) {
			SDL_Rect box = { x, y, w, h };
			SDL_RenderCopy(renderer, texture, NULL, &box);
		}
	}
} // namespace flat2d
