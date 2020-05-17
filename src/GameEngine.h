#ifndef GAMEENGINE_H_
#define GAMEENGINE_H_

#include <functional>

namespace flat2d {
	class GameData;
	class RenderData;
	class EntityContainer;

	/**
	 * Available returns for the StateCallback
	 */
	enum GameStateAction
	{
		NOOP,
		RESET,
		QUIT
	};

	typedef std::function<void(const SDL_Event&)> HandleCallback;
	typedef std::function<GameStateAction(GameData*)> StateCallback;

	/**
	 * The game engine. Use this to start the game loop
	 */
	class GameEngine
	{
	  private:
		GameData* gameData;

		int screenTicksPerFrame = 1000 / 60;

		GameEngine(const GameEngine&);     // Don't implement
		void operator=(const GameEngine&); // Don't implement

	  public:
		/**
		 * Construct the GameEngine
		 * @param data The GameData*
		 */
		explicit GameEngine(GameData* data)
		  : gameData(data)
		{}

		/**
		 * Empty destructor. Currently destroying the game engine has
		 * no additional impact on flat. You shouldn't call this though.
		 */
		~GameEngine() {}

		/**
		 * Init the GameEngine with the provded fps
		 * @param fps Game fps
		 */
		void init(int fps);

		/**
		 * Start the game loop
		 *
		 * @param stateCallback Optional callback to handle the game loop
		 * @param handleCallback Optional callback to handle SDL_Events
		 */
		void run(StateCallback = nullptr, HandleCallback = nullptr) const;
	};
} // namespace flat2d

#endif // GAMEENGINE_H_
