#include "Window.h"
#include <iostream>

namespace flat2d {
	bool Window::init()
	{
		window = SDL_CreateWindow(title.c_str(),
		                          SDL_WINDOWPOS_UNDEFINED,
		                          SDL_WINDOWPOS_UNDEFINED,
		                          width,
		                          height,
		                          SDL_WINDOW_SHOWN);

		if (window == nullptr) {
			std::cerr << "Window could not be created: " << SDL_GetError()
			          << std::endl;
			return false;
		}

		renderer = SDL_CreateRenderer(
		  window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		if (renderer == nullptr) {
			std::cerr << "Renderer could not be created: " << SDL_GetError()
			          << std::endl;
			return false;
		}

		return true;
	}
} // namespace flat2d
