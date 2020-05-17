#include <SDL.h>

#include "../src/Camera.h"
#include "catch.hpp"

TEST_CASE("CameraTests", "[camera]")
{
	flat2d::Camera* camera = new flat2d::Camera(200, 100);
	camera->setMapDimensions(1000, 100);

	SECTION("Map dimensions", "[camera][map]")
	{
		camera->setMapDimensions(1000, 100);
		REQUIRE(camera->getMapWidth() == 1000);
		REQUIRE(camera->getMapHeight() == 100);
		camera->setMapDimensions(2000, 200);
		REQUIRE(camera->getMapWidth() == 2000);
		REQUIRE(camera->getMapHeight() == 200);
	}

	SECTION("Camera dimensions", "[camera]")
	{
		REQUIRE(camera->getHeight() == 100);
		REQUIRE(camera->getWidth() == 200);

		SDL_Rect box = camera->getBox();
		REQUIRE(box.h == 100);
		REQUIRE(box.w == 200);
	}

	SECTION("Is visible on camera", "[camera]")
	{
		SDL_Rect box = { 0, 0, 50, 50 };
		REQUIRE(camera->isVisibleOnCamera(box));
		box.x = -25;
		box.y = -25;
		REQUIRE(camera->isVisibleOnCamera(box));
		box.x = 201;
		box.y = 101;
		REQUIRE(!camera->isVisibleOnCamera(box));
		box.x = 199;
		box.y = 99;
		REQUIRE(camera->isVisibleOnCamera(box));
	}

	SECTION("Center camera", "[camera]")
	{
		SDL_Rect box;
		camera->setMapDimensions(1000, 1000);

		camera->centerOn(500, 500);
		REQUIRE(camera->getXpos() == 400);
		REQUIRE(camera->getYpos() == 450);
		box = camera->getBox();
		REQUIRE(box.x == 400);
		REQUIRE(box.y == 450);

		camera->centerOn(1000, 1000);
		REQUIRE(camera->getXpos() == 800);
		REQUIRE(camera->getYpos() == 900);
		box = camera->getBox();
		REQUIRE(box.x == 800);
		REQUIRE(box.y == 900);

		camera->centerOn(10, 10);
		REQUIRE(camera->getXpos() == 0);
		REQUIRE(camera->getYpos() == 0);
		box = camera->getBox();
		REQUIRE(box.x == 0);
		REQUIRE(box.y == 0);
	}

	SECTION("Test map bounds", "[camera]")
	{
		SDL_Rect box = { 0, 0, 200, 200 };
		camera->setMapDimensions(1000, 1000);

		REQUIRE(!camera->isOutOfMapBounds(box));
		box.x = 1001;
		REQUIRE(camera->isOutOfMapBounds(box));
		box.x = 950;
		REQUIRE(!camera->isOutOfMapBounds(box));
		box.x = 950;
		box.y = 950;
		REQUIRE(!camera->isOutOfMapBounds(box));
		box.x = 200;
		REQUIRE(!camera->isOutOfMapBounds(box));
		box.y = 200;
		REQUIRE(!camera->isOutOfMapBounds(box));
		box.x = -201;
		box.y = -201;
		REQUIRE(camera->isOutOfMapBounds(box));
		box.x = 1001;
		box.y = 1001;
		REQUIRE(camera->isOutOfMapBounds(box));
	}

	delete camera;
}
