#include <SDL.h>
#include <SDL_ttf.h>

#include "DeltatimeMonitor.h"
#include "EntityContainer.h"
#include "GameData.h"
#include "GameEngine.h"
#include "RenderData.h"
#include "Timer.h"

namespace flat2d {
	void GameEngine::init(int fps)
	{
		int nfps = fps > 0 ? fps : 60;
		this->screenTicksPerFrame = 1000 / nfps;
	}

	void GameEngine::run(StateCallback stateCallback,
	                     HandleCallback handleCallback) const
	{
		SDL_Renderer* renderer = gameData->getRenderData()->getRenderer();
		EntityContainer* entityContainer = gameData->getEntityContainer();

		// Loop stuff
		flat2d::Timer fpsCapTimer;
		SDL_Event e;
		bool quit = false;

		// Main loop
		gameData->getDeltatimeMonitor()->updateDeltaTime();
		while (!quit) {
			fpsCapTimer.start();
			gameData->getDeltatimeMonitor()->updateDeltaTime();

			if (stateCallback) {
				switch (stateCallback(gameData)) {
					case RESET:
						continue;
					case QUIT:
						quit = true;
						break;
					case NOOP:
					default:
						break;
				}
			}

			entityContainer->initiateEntities(gameData);

			// Handle events
			while (SDL_PollEvent(&e) != 0) {
				if (e.type == SDL_QUIT) {
					quit = true;
					break;
				}
				if (handleCallback) {
					handleCallback(e);
				}
				entityContainer->handleObjects(e, gameData);
			}

			entityContainer->moveObjects(gameData);

			// Clear screen to black
			SDL_SetRenderDrawColor(renderer, 0x0, 0x0, 0x0, 0xFF);
			SDL_RenderClear(renderer);
			entityContainer->renderObjects(gameData);

			SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);

			// Update the screen
			SDL_RenderPresent(renderer);

			int tickCount = fpsCapTimer.getTicks();
			if (tickCount < screenTicksPerFrame) {
				SDL_Delay(screenTicksPerFrame - tickCount);
			}
		}
	}
} // namespace flat2d
