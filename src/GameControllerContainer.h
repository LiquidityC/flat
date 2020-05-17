#ifndef GAMECONTROLLERCONTAINER_H_
#define GAMECONTROLLERCONTAINER_H_

#include "GameController.h"
#include <vector>

namespace flat2d {
	/**
	 * Container for connected game controllers
	 * @author Linus Probert <linus.probert@gmail.com>
	 */
	class GameControllerContainer
	{
	  private:
		std::vector<GameController*> controllers;

		int loadControllers();

	  public:
		GameControllerContainer();
		virtual ~GameControllerContainer();

		/**
		 * Check if any controllers exist
		 * @return true or false
		 */
		bool hasControllers();

		/**
		 * Get a controller object
		 * @param index the GameControllers index
		 * @return A GameController object
		 */
		GameController* getController(int index);
	};
} // namespace flat2d

#endif // GAMECONTROLLERCONTAINER_H_
