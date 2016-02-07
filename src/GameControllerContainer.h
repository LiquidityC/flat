#ifndef GAMECONTROLLERCONTAINER_H_
#define GAMECONTROLLERCONTAINER_H_

#include <vector>
#include "GameController.h"

namespace flat2d
{
	class GameControllerContainer
	{
		private:
			std::vector<GameController*> controllers;

			int loadControllers();

		public:
			GameControllerContainer();
			virtual ~GameControllerContainer();

			bool hasControllers();
			GameController* getController(int index);
	};
} // namespace flat2d

#endif // GAMECONTROLLERCONTAINER_H_
