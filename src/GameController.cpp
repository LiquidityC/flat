#include "GameController.h"
#include <iostream>

namespace flat2d {
	GameController::GameController(int index)
	{
		controller = SDL_GameControllerOpen(index);
		if (controller == nullptr) {
			std::cout << "Unable to open controller (" << index << ")"
			          << std::endl;
		}
	}

	GameController::~GameController()
	{
		if (controller != nullptr) {
			SDL_GameControllerClose(controller);
		}
	}

	SDL_GameController* GameController::getSDLController()
	{
		return controller;
	}
} // namespace flat2d
