#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "GameEngine.h"
#include "GameData.h"
#include "EntityContainer.h"
#include "RenderData.h"
#include "Timer.h"
#include "DeltatimeMonitor.h"
#include "VirtualGameStateController.h"

namespace flat2d
{
	void GameEngine::init(int screenWidth, int screenHeight, int fps)
	{
		this->screenWidth = screenWidth;
		this->screenHeight = screenHeight;
		this->screenFps = fps;
		this->screenTicksPerFrame = 1000 / fps;
	}

	void GameEngine::run(VirtualGameStateController *gameStateController) const
	{
		SDL_Renderer *renderer = gameData->getRenderData()->getRenderer();
		EntityContainer *entityContainer = gameData->getEntityContainer();

		Timer fpsTimer;
		Timer drawFpsTimer;

#ifdef DEBUG
		int countedFrames = 0;
		int currentFps = 0;
		float avgFps = 0;
#endif

		fpsTimer.start();
		drawFpsTimer.start();

		// Loop stuff
		flat2d::Timer fpsCapTimer;;
		SDL_Event e;
		bool quit = false;

		// Main loop
		gameData->getDeltatimeMonitor()->updateDeltaTime();
		while (!quit) {
			fpsCapTimer.start();
			gameData->getDeltatimeMonitor()->updateDeltaTime();

			if (gameStateController) {
				if (gameStateController->gameStateCheck(gameData)) {
					continue;
				}
			}

			// Handle events
			while (SDL_PollEvent (&e) != 0) {
				if (e.type == SDL_QUIT) {
					quit = true;
					break;
				}
				if (gameStateController) {
					gameStateController->handle(e);
				}
				entityContainer->handleObjects(e, gameData);
			}

			entityContainer->moveObjects(gameData);

			// Clear screen to black
			SDL_SetRenderDrawColor( renderer, 0x0, 0x0, 0x0, 0xFF );
			SDL_RenderClear( renderer );
			entityContainer->renderObjects(gameData);

			SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);

			// Update the screen
			SDL_RenderPresent( renderer );

#ifdef DEBUG
			avgFps = countedFrames / (fpsTimer.getTicks() / 1000.f);
			if (avgFps > 20000) {
				avgFps = 0;
			}
			if (static_cast<int>((drawFpsTimer.getTicks() / 1000.f)) > 1) {
				std::cout << "AVG FPS: " << avgFps << " FPS: " << currentFps
					<< " DT: " << gameData->getDeltatimeMonitor()->getDeltaTime() << std::endl;
				currentFps = 0;
				drawFpsTimer.stop();
				drawFpsTimer.start();
			}
			countedFrames++;
			currentFps++;
#endif

			int tickCount = fpsCapTimer.getTicks();
			if (tickCount < screenTicksPerFrame) {
				SDL_Delay(screenTicksPerFrame - tickCount);
			}
		}
	}
} // namespace flat2d
