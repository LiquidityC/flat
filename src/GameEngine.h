#ifndef GAMEENGINE_H_
#define GAMEENGINE_H_

namespace flat2d
{
	class GameData;
	class RenderData;
	class EntityContainer;
	class VirtualGameStateController;

	class GameEngine
	{
		private:
			GameData *gameData;

			int screenWidth = 800;
			int screenHeight = 600;
			int screenFps = 60;
			int screenTicksPerFrame = 1000/60;

			GameEngine(const GameEngine&); // Don't implement
			void operator=(const GameEngine&); // Don't implement

		public:
			explicit GameEngine(GameData *data) : gameData(data) { }
			~GameEngine() { }

			void init(int screenWidth, int screenHeight, int fps);

			void run(VirtualGameStateController *gameStateController) const;
	};
} // namespace flat2d

#endif // GAMEENGINE_H_
