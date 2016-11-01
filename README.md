# flat (2D game engine) [![Build Status](https://travis-ci.org/LiquidityC/flat.svg?branch=master)](https://travis-ci.org/LiquidityC/flat)

This is my 2D game engine

## Features
- Simple collision handling
- Parallax effects
- Music and sound
- Game controller support
- hiDPI screen support

## Compiling
Before compiling you need to install SDL2, SDL2_image, SDL2_mixer and SDL2_ttf

- Clone the project and navigate to the root folder
- mkdir build
- cd build
- cmake ..
- make

## Getting started
```c++
#include <iostream>
#include <flat/flat.h>

int main(int argc, char* argv[])
{
	std::cout << "Loading Flat" << std::endl;
	flat2d::FlatBuilder *flat = new flat2d::FlatBuilder();
	flat->loadSDL("Flat Demo", 60, 800, 600);
	flat2d::GameEngine *engine = flat->getGameEngine();

	flat2d::EntityContainer *container = flat->getGameData()->getEntityContainer();
    // You can register your objects to the container here. Objects extend the Entity class in flat

	// Create the callback methods
	auto stateCallback = [](flat2d::GameData *gameData) -> flat2d::GameStateAction {
		return flat2d::GameStateAction::NOOP;
	};
	auto handleCallback = [](const SDL_Event& event) -> void {
		// Nothing to do here
	};

	std::cout << "Starting the engine" << std::endl;
    // stateCallback and handleCallback can be omitted if you don't need them
	engine->run(stateCallback, handleCallback);
	std::cout << "Exiting" << std::endl;

	delete flat;
}
```

## Documentation
[API](http://liquidityc.github.io/flat)
