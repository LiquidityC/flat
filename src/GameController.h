#ifndef GAMECONTROLLER_H_
#define GAMECONTROLLER_H_

#include <SDL.h>

namespace flat2d {
	class GameControllerContainer;

	/**
	 * A helper class to add game controllers/pads to your game
	 * @author Linus Probert <linus.probert@gmail.com>
	 */
	class GameController
	{
	  private:
		SDL_GameController* controller = nullptr;

	  public:
		explicit GameController(int index);

		~GameController();

		SDL_GameController* getSDLController();
	};
} // namespace flat2d

#endif // GAMECONTROLLER_H_
