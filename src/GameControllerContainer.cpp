#include <iostream>

#include "GameController.h"
#include "GameControllerContainer.h"

namespace flat2d {
	GameControllerContainer::GameControllerContainer() { loadControllers(); }

	GameControllerContainer::~GameControllerContainer()
	{
		for (auto it = controllers.begin(); it != controllers.end(); it++) {
			delete *it;
		}
	}

	int GameControllerContainer::loadControllers()
	{
		int controllerCount = SDL_NumJoysticks();
		if (controllerCount < 1) {
			std::cout << "No game controller available" << std::endl;
			return controllerCount;
		}

		for (int i = 0; i < controllerCount; i++) {
			GameController* controller = new GameController(i);

			if (controller->getSDLController() != nullptr) {
				controllers.push_back(controller);
			} else {
				delete controller;
			}
		}

		return controllerCount;
	}

	bool GameControllerContainer::hasControllers()
	{
		return !controllers.empty();
	}

	GameController* GameControllerContainer::getController(int index)
	{
		return controllers[index];
	}
} // namespace flat2d
