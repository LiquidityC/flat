#ifndef FLATBUILDER_H_
#define FLATBUILDER_H_

#include <string>

namespace flat2d {
	class EntityContainer;
	class CollisionDetector;
	class Window;
	class Camera;
	class GameData;
	class RenderData;
	class Mixer;
	class DeltatimeMonitor;
	class GameControllerContainer;
	class GameEngine;

	/**
	 * This is the main builder for the flat library. It is responsible
	 * for setting up all the services and containers.
	 */
	class FlatBuilder
	{
	  private:
		Window* window = nullptr;
		RenderData* renderData = nullptr;
		GameData* gameData = nullptr;
		CollisionDetector* collisionDetector = nullptr;
		EntityContainer* entityContainer = nullptr;
		Camera* camera = nullptr;
		Mixer* mixer = nullptr;
		DeltatimeMonitor* deltatimeMonitor = nullptr;
		GameControllerContainer* controllerContainer = nullptr;
		GameEngine* gameEngine = nullptr;

		bool hidpi = false;

		/**
		 * Inits SDL. Creating a window according to provided dimension
		 * with the provided name.
		 *
		 * @param title The window name
		 * @param screenWidth The window width
		 * @param screenHeight The window height
		 * @return success or failure bool
		 */
		bool initSDL(std::string, int, int);

		/**
		 * Initiate the containers. Gives all pointer values and creates
		 * all the needed services.
		 *
		 * @return bool success or failure
		 */
		bool initContainers();

	  public:
		/**
		 * Initiate the engine and all included systems.
		 *
		 * @return int 0 if success
		 */
		int loadSDL(const std::string& name,
		            int fps,
		            int screenWidth,
		            int screenHeight);

		/**
		 * Cleanly destroy all related objects and shut down SDL nicely.
		 */
		~FlatBuilder();

		/**
		 * Returns the GameData pointer.
		 *
		 * @return GameData*
		 */
		GameData* getGameData() const;

		/**
		 * Returns the GameEngine pointer
		 *
		 * @return GameEngine*
		 */
		GameEngine* getGameEngine() const;

		/**
		 * Set HiDPI mode for the game
		 *
		 * @param hidpi true or false
		 */
		void setHiDPI(bool hidpi) { this->hidpi = hidpi; }
	};
} // namespace flat2d

#endif // FLATBUILDER_H_

/*! \mainpage The Flat library
 *	\section Intro
 *	Flat is a 2D game engine wrapping SDL2.0. It's a work in progress but
 *the basic 2D engine is more or less fully functional. \section Features
 *	- Simple collision handling
 *	- Parallax scrolling
 *	- Music and soundeffects
 *	- Gamecontroller input (xbox/ps4/etc)
 *	- hiDPI screens
 */
