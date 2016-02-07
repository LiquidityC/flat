#ifndef GAMECONTROLLER_H_
#define GAMECONTROLLER_H_

#include <SDL2/SDL.h>

namespace flat2d
{
	class GameControllerContainer;

	class GameController
	{
		private:
			SDL_GameController *controller = nullptr;

		public:
			explicit GameController(int index);

			~GameController();

			SDL_GameController* getSDLController();
	};
} // namespace flat2d

#endif // GAMECONTROLLER_H_
